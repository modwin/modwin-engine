#ifndef MODWIN_ENGINE_MAPPARSER_H
#define MODWIN_ENGINE_MAPPARSER_H

#include "graphics/TileMap.h"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tinyxml2.h>

namespace Modwin
{
	class MapParser
	{
	public:
		static MapParser* GetInstance();

		void Clean();
		bool LoadMap(const std::string& id, const std::string& fileName);
		[[nodiscard]] TileMap* GetTileMap(const std::string& id) noexcept;
		bool Parse(const std::string& id, const std::string& source);

		static std::optional<Tileset> ParseTileSet(const tinyxml2::XMLElement* tilesetElement);
		static std::optional<TileLayer> ParseTileLayer(
			const tinyxml2::XMLElement* layerElement, int mapTileWidth);

	private:
		MapParser() = default;

		std::map<std::string, std::unique_ptr<TileMap>> m_Maps;
	};
}

#endif // MODWIN_ENGINE_MAPPARSER_H
