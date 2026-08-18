#include "graphics/MapSerializer.h"

#include "Log.h"

#include <sstream>
#include <system_error>
#include <tinyxml2.h>

namespace Modwin
{
	namespace
	{
		bool ReplaceFileWithBackup(
			const std::filesystem::path& temporaryPath,
			const std::filesystem::path& destination)
		{
			std::error_code error;
			if (!std::filesystem::exists(destination, error))
			{
				std::filesystem::rename(temporaryPath, destination, error);
				return !error;
			}

			std::filesystem::path backupPath = destination;
			backupPath += ".bak";
			std::filesystem::copy_file(
				destination,
				backupPath,
				std::filesystem::copy_options::overwrite_existing,
				error);
			if (error)
			{
				return false;
			}

			std::filesystem::remove(destination, error);
			if (error)
			{
				return false;
			}

			std::filesystem::rename(temporaryPath, destination, error);
			if (error)
			{
				std::error_code restoreError;
				std::filesystem::copy_file(
					backupPath,
					destination,
					std::filesystem::copy_options::overwrite_existing,
					restoreError);
				return false;
			}

			std::filesystem::remove(backupPath, error);
			return true;
		}
	}

	bool MapSerializer::Save(
		const TileMap& map, const std::filesystem::path& destination)
	{
		if (destination.empty() || map.GetWidth() <= 0 || map.GetHeight() <= 0 ||
			map.GetTileWidth() <= 0 || map.GetTileHeight() <= 0)
		{
			Log::GetCoreLogger()->error("Cannot save a map with an invalid path or dimensions.");
			return false;
		}

		tinyxml2::XMLDocument document;
		document.InsertEndChild(document.NewDeclaration());

		auto* mapElement = document.NewElement("map");
		mapElement->SetAttribute("version", "1.11");
		mapElement->SetAttribute("orientation", "orthogonal");
		mapElement->SetAttribute("renderorder", "right-down");
		mapElement->SetAttribute("width", map.GetWidth());
		mapElement->SetAttribute("height", map.GetHeight());
		mapElement->SetAttribute("tilewidth", map.GetTileWidth());
		mapElement->SetAttribute("tileheight", map.GetTileHeight());
		mapElement->SetAttribute("infinite", 0);
		mapElement->SetAttribute("nextlayerid", static_cast<unsigned int>(map.GetLayers().size() + 1U));
		document.InsertEndChild(mapElement);

		for (const Tileset& tileset : map.GetTilesets())
		{
			auto* tilesetElement = document.NewElement("tileset");
			tilesetElement->SetAttribute("firstgid", tileset.firstId);
			tilesetElement->SetAttribute("name", tileset.name.c_str());
			tilesetElement->SetAttribute("tilewidth", tileset.tileWidth);
			tilesetElement->SetAttribute("tileheight", tileset.tileHeight);
			tilesetElement->SetAttribute("tilecount", tileset.tileCount);
			tilesetElement->SetAttribute("columns", tileset.columnCount);

			auto* imageElement = document.NewElement("image");
			imageElement->SetAttribute("source", tileset.source.c_str());
			tilesetElement->InsertEndChild(imageElement);
			mapElement->InsertEndChild(tilesetElement);
		}

		int layerId = 1;
		for (const TileLayer& layer : map.GetLayers())
		{
			auto* layerElement = document.NewElement("layer");
			layerElement->SetAttribute("id", layerId++);
			layerElement->SetAttribute("name", layer.GetName().c_str());
			layerElement->SetAttribute("width", layer.GetWidth());
			layerElement->SetAttribute("height", layer.GetHeight());

			std::ostringstream csv;
			const auto& tiles = layer.GetTiles();
			for (int row = 0; row < layer.GetHeight(); ++row)
			{
				for (int column = 0; column < layer.GetWidth(); ++column)
				{
					const auto index = static_cast<std::size_t>(row) *
						static_cast<std::size_t>(layer.GetWidth()) +
						static_cast<std::size_t>(column);
					csv << tiles[index];
					if (column + 1 < layer.GetWidth() || row + 1 < layer.GetHeight())
					{
						csv << ',';
					}
				}
				csv << '\n';
			}

			auto* dataElement = document.NewElement("data");
			dataElement->SetAttribute("encoding", "csv");
			dataElement->SetText(csv.str().c_str());
			layerElement->InsertEndChild(dataElement);
			mapElement->InsertEndChild(layerElement);
		}

		std::filesystem::path temporaryPath = destination;
		temporaryPath += ".tmp";
		if (document.SaveFile(temporaryPath.string().c_str()) != tinyxml2::XML_SUCCESS)
		{
			Log::GetCoreLogger()->error("Could not write temporary map '{}'.", temporaryPath.string());
			return false;
		}

		if (!ReplaceFileWithBackup(temporaryPath, destination))
		{
			Log::GetCoreLogger()->error("Could not replace map '{}'.", destination.string());
			return false;
		}

		Log::GetCoreLogger()->info("Saved map '{}'.", destination.string());
		return true;
	}
}
