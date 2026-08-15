#include "graphics/MapParser.h"
#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include "Log.h"
#include "core/ResourcePaths.h"
#include <optional>

namespace Modwin
{
	MapParser* MapParser::s_INSTANCE = nullptr;

	MapParser* MapParser::GetInstance()
	{
		if (s_INSTANCE == nullptr)
		{
			s_INSTANCE = new MapParser();
		}
		return s_INSTANCE;
	}

	void MapParser::Clean()
	{
		std::map<std::string, TileMap*>::iterator iter;
		for (iter = m_Map.begin(); iter != m_Map.end(); iter++)
		{
			iter->second = nullptr;
		}
		m_Map.clear();
	}

	bool MapParser::LoadMap(const std::string& id, const std::string& fileName)
	{
		const auto mapPath = GetResourcePath("maps") / (fileName + ".tmx");
		return Parse(id, mapPath.string());
	}

	/**
	 * Funktion för att läsa XML-filen och hämta datan från den för att
	 * sedan kunna mata in värdena i nästa funktion.
	 *
	 */
	bool MapParser::Parse(const std::string& id, const std::string& source)
	{
		tinyxml2::XMLDocument xmlDocument;
		xmlDocument.LoadFile(source.c_str());
		if (xmlDocument.Error())
		{
			Log::GetCoreLogger()->info(xmlDocument.ErrorStr());
		}

		tinyxml2::XMLElement* rootElement = xmlDocument.RootElement();
		if (rootElement == nullptr)
		{
			Log::GetCoreLogger()->info("MapParser: rootElement was null.");
			return false;
		}
		int rowCount, columnCount, tileWidth = 0;

		// Hämtar attributen av varje element i XML-filen som behövs för
		// att rendera texturerna.
		columnCount = rootElement->FindAttribute("width")->IntValue();
		rowCount = rootElement->FindAttribute("height")->IntValue();
		tileWidth = rootElement->FindAttribute("tilewidth")->IntValue();

		TilesetVec tilesets;
		const tinyxml2::XMLElement* e = rootElement->FirstChildElement();

		for (const tinyxml2::XMLElement* element =
				     e;
		     element != nullptr;
		     element = element->NextSiblingElement("tileset"))
		{
			auto tileset = ParseTileSet(element);

			if (!tileset.has_value()) return false;
			tilesets.push_back(std::move(*tileset));
		}

		auto* tileMap = new TileMap();
		TileLayer* tileLayer = nullptr;
		e = rootElement->FirstChildElement();
		while (e)
		{
			if (e->Value() == std::string("layer"))
			{
				tileLayer = ParseTileLayer(e, tilesets, tileWidth, rowCount, columnCount);
				tileMap->m_SurfaceLayers.push_back(tileLayer);
			}
			e = e->NextSiblingElement();
		}

		if (tileMap->m_SurfaceLayers.empty())
		{
			Log::GetCoreLogger()->info("SurfaceLayers<vec> är tom!");
			//			return false;
		}

		m_Map[id] = tileMap;
		if (m_Map[id] == nullptr)
		{
			std::cout << "Map is null!" << std::endl;
			return false;
		}
		return true;
	}

	std::optional<Tileset> MapParser::ParseTileSet(const tinyxml2::XMLElement* tilesetElement)
	{
		const char* name = tilesetElement->Attribute("name");
		const tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");

		if (imageElement == nullptr) return std::nullopt;

		const char* source = imageElement->Attribute("source");
		unsigned int firstId = 0;
		int tileCount, columnCount, tileWidth = 0;
		const bool valid =
				name != nullptr &&
				source != nullptr &&
				tilesetElement->QueryUnsignedAttribute(
					"firstgid", &firstId) == tinyxml2::XML_SUCCESS &&
				tilesetElement->QueryIntAttribute(
					"tilecount", &tileCount) == tinyxml2::XML_SUCCESS &&
				tilesetElement->QueryIntAttribute(
					"columns", &columnCount) == tinyxml2::XML_SUCCESS &&
				tilesetElement->QueryIntAttribute(
					"tilewidth", &tileWidth) == tinyxml2::XML_SUCCESS &&
				tileCount > 0 &&
				columnCount > 0 &&
				tileWidth > 0;

		if (!valid)
		{
			Log::GetCoreLogger()->info("Map contains an invalid tileset definition.");
			return std::nullopt;
		}
		Tileset tileset;
		tileset.name = name;
		tileset.source = source;
		tileset.tileCount = tileCount;
		tileset.columnCount = columnCount;
		tileset.rowCount = tileCount / columnCount;
		tileset.tileWidth = tileWidth;
		tileset.firstId = firstId;
		tileset.lastId = (tileset.firstId + static_cast<TileGid>(tileset.tileCount) - 1);

		return tileset;
	}

	TileLayer*
	MapParser::ParseTileLayer(tinyxml2::XMLElement* layerElement,
		const TilesetVec& tilesetVec, int tileWidth, int rowCount, int columnCount)
	{
		tinyxml2::XMLElement* data = layerElement->FirstChildElement();
		while (data)
		{
			if (data->Value() == std::string("data"))
			{
				break;
			}
			data = data->NextSiblingElement();
		}

		if (data == nullptr)
		{
			return nullptr;
		}

		std::string matrix(data->GetText());
		std::istringstream iss(matrix);
		std::string id;

		TileVec2D tileVec(rowCount, std::vector<int>(columnCount, 0));

		for (int row = 0; row < rowCount; row++)
		{
			for (int column = 0; column < columnCount; column++)
			{
				std::getline(iss, id, ',');
				std::stringstream converter(id);
				converter >> tileVec[row][column];
				if (iss.bad())
				{
					break;
				}
			}
		}
		return new TileLayer(tileWidth, rowCount, columnCount, tileVec, tilesetVec);
		//
		//
		//		for(tinyxml2::XMLElement* e = layerElement->FirstChildElement(); e!= nullptr;e->NextSibling())
		//		{
		//			if(e->Value() == std::string("data"))
		//			{
		//				dataElement = e;
		//				break;
		//			}
		//		}
		return nullptr;
	}
}


