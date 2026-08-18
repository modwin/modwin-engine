#include "graphics/MapParser.h"

#include "Log.h"
#include "core/ResourcePaths.h"

#include <cstdint>
#include <exception>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Modwin
{
	MapParser* MapParser::GetInstance()
	{
		static MapParser instance;
		return &instance;
	}

	void MapParser::Clean()
	{
		m_Maps.clear();
	}

	bool MapParser::LoadMap(const std::string& id, const std::string& fileName)
	{
		const auto mapPath = GetResourcePath("maps") / (fileName + ".tmx");
		return Parse(id, mapPath.string());
	}

	TileMap* MapParser::GetTileMap(const std::string& id) noexcept
	{
		const auto map = m_Maps.find(id);
		return map != m_Maps.end() ? map->second.get() : nullptr;
	}

	bool MapParser::Parse(const std::string& id, const std::string& source)
	{
		auto tileMap = LoadFromFile(source);
		if (!tileMap.has_value())
		{
			return false;
		}

		m_Maps.insert_or_assign(id, std::make_unique<TileMap>(std::move(*tileMap)));
		return true;
	}

	std::optional<TileMap> MapParser::LoadFromFile(const std::filesystem::path& source)
	{
		tinyxml2::XMLDocument xmlDocument;
		if (xmlDocument.LoadFile(source.string().c_str()) != tinyxml2::XML_SUCCESS)
		{
			Log::GetCoreLogger()->error(
				"Could not load map '{}': {}", source.string(), xmlDocument.ErrorStr());
			return std::nullopt;
		}

		const tinyxml2::XMLElement* rootElement = xmlDocument.RootElement();
		if (rootElement == nullptr)
		{
			Log::GetCoreLogger()->error("Map '{}' has no root element.", source.string());
			return std::nullopt;
		}

		int mapWidth = 0;
		int mapHeight = 0;
		int mapTileWidth = 0;
		int mapTileHeight = 0;
		if (rootElement->QueryIntAttribute("width", &mapWidth) != tinyxml2::XML_SUCCESS ||
			rootElement->QueryIntAttribute("height", &mapHeight) != tinyxml2::XML_SUCCESS ||
			rootElement->QueryIntAttribute("tilewidth", &mapTileWidth) != tinyxml2::XML_SUCCESS ||
			rootElement->QueryIntAttribute("tileheight", &mapTileHeight) != tinyxml2::XML_SUCCESS ||
			mapWidth <= 0 || mapHeight <= 0 || mapTileWidth <= 0 || mapTileHeight <= 0)
		{
			Log::GetCoreLogger()->error("Map '{}' has invalid dimensions.", source.string());
			return std::nullopt;
		}

		TileMap tileMap(mapWidth, mapHeight, mapTileWidth, mapTileHeight);

		for (const tinyxml2::XMLElement* element = rootElement->FirstChildElement("tileset");
			element != nullptr;
			element = element->NextSiblingElement("tileset"))
		{
			auto tileset = ParseTileSet(element);
			if (!tileset.has_value())
			{
				return std::nullopt;
			}

			tileMap.AddTileset(std::move(*tileset));
		}

		for (const tinyxml2::XMLElement* element = rootElement->FirstChildElement("layer");
			element != nullptr;
			element = element->NextSiblingElement("layer"))
		{
			auto layer = ParseTileLayer(element, mapTileWidth);
			if (!layer.has_value())
			{
				return std::nullopt;
			}

			tileMap.AddLayer(std::move(*layer));
		}

		if (tileMap.GetLayers().empty())
		{
			Log::GetCoreLogger()->error("Map '{}' contains no tile layers.", source.string());
			return std::nullopt;
		}

		return tileMap;
	}

	std::optional<Tileset> MapParser::ParseTileSet(
		const tinyxml2::XMLElement* tilesetElement)
	{
		if (tilesetElement == nullptr)
		{
			return std::nullopt;
		}

		const char* name = tilesetElement->Attribute("name");
		const tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");
		const char* imageSource = imageElement != nullptr ? imageElement->Attribute("source") : nullptr;

		unsigned int firstId = 0;
		int tileCount = 0;
		int columnCount = 0;
		int tileWidth = 0;
		int tileHeight = 0;

		const bool valid =
			name != nullptr &&
			imageSource != nullptr &&
			tilesetElement->QueryUnsignedAttribute("firstgid", &firstId) == tinyxml2::XML_SUCCESS &&
			tilesetElement->QueryIntAttribute("tilecount", &tileCount) == tinyxml2::XML_SUCCESS &&
			tilesetElement->QueryIntAttribute("columns", &columnCount) == tinyxml2::XML_SUCCESS &&
			tilesetElement->QueryIntAttribute("tilewidth", &tileWidth) == tinyxml2::XML_SUCCESS &&
			tilesetElement->QueryIntAttribute("tileheight", &tileHeight) == tinyxml2::XML_SUCCESS &&
			tileCount > 0 && columnCount > 0 && tileWidth > 0 && tileHeight > 0;

		if (!valid)
		{
			Log::GetCoreLogger()->error("Map contains an invalid tileset definition.");
			return std::nullopt;
		}

		Tileset tileset;
		tileset.name = name;
		tileset.source = imageSource;
		tileset.firstId = static_cast<TileGid>(firstId);
		tileset.tileCount = tileCount;
		tileset.columnCount = columnCount;
		tileset.rowCount = (tileCount + columnCount - 1) / columnCount;
		tileset.tileWidth = tileWidth;
		tileset.tileHeight = tileHeight;
		tileset.lastId = tileset.firstId + static_cast<TileGid>(tileset.tileCount) - 1U;

		return tileset;
	}

	std::optional<TileLayer> MapParser::ParseTileLayer(
		const tinyxml2::XMLElement* layerElement, const int mapTileWidth)
	{
		if (layerElement == nullptr)
		{
			return std::nullopt;
		}

		const char* name = layerElement->Attribute("name");
		int width = 0;
		int height = 0;
		if (name == nullptr ||
			layerElement->QueryIntAttribute("width", &width) != tinyxml2::XML_SUCCESS ||
			layerElement->QueryIntAttribute("height", &height) != tinyxml2::XML_SUCCESS ||
			width <= 0 || height <= 0)
		{
			Log::GetCoreLogger()->error("Map contains an invalid tile-layer definition.");
			return std::nullopt;
		}

		const tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
		if (dataElement == nullptr || dataElement->GetText() == nullptr)
		{
			Log::GetCoreLogger()->error("Tile layer '{}' contains no data.", name);
			return std::nullopt;
		}

		const char* encoding = dataElement->Attribute("encoding");
		if (encoding == nullptr || std::string(encoding) != "csv")
		{
			Log::GetCoreLogger()->error("Tile layer '{}' must use CSV encoding.", name);
			return std::nullopt;
		}

		const auto expectedCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
		std::vector<TileGid> tiles;
		tiles.reserve(expectedCount);

		std::istringstream stream(dataElement->GetText());
		std::string token;
		try
		{
			while (std::getline(stream, token, ','))
			{
				const auto firstCharacter = token.find_first_not_of(" \t\r\n");
				if (firstCharacter == std::string::npos)
				{
					continue;
				}

				std::size_t parsedCharacters = 0;
				const unsigned long long value = std::stoull(token, &parsedCharacters);
				if (token.find_first_not_of(" \t\r\n", parsedCharacters) != std::string::npos ||
					value > std::numeric_limits<TileGid>::max())
				{
					throw std::out_of_range("Invalid tile GID");
				}

				tiles.push_back(static_cast<TileGid>(value));
			}
		}
		catch (const std::exception& exception)
		{
			Log::GetCoreLogger()->error(
				"Could not parse tile data for layer '{}': {}", name, exception.what());
			return std::nullopt;
		}

		if (tiles.size() != expectedCount)
		{
			Log::GetCoreLogger()->error(
				"Layer '{}' contains {} tiles, but its dimensions require {}.",
				name, tiles.size(), expectedCount);
			return std::nullopt;
		}

		return TileLayer(name, width, height, mapTileWidth, std::move(tiles));
	}
}
