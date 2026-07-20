//
// Created by komvu on 2025-01-04.
//

#ifndef WINTHER_ENGINE_MAPPARSER_H
#define WINTHER_ENGINE_MAPPARSER_H

#include <map>
#include <graphics/TileMap.h>
#include <graphics/TileLayer.h>
#include <tinyxml2.h>

namespace Winther
{
	class MapParser
	{
	public:
		static MapParser* GetInstance();

		void Clean();
		bool LoadMap(const std::string& id, const std::string& fileName);

		inline TileMap* GetTileMap(const std::string& id){return m_Map[id];}

		bool Parse(const std::string& id, const std::string& source);
		Tileset ParseTileSet(tinyxml2::XMLElement* tilesetElement);
		TileLayer* ParseTileLayer(tinyxml2::XMLElement* layerElement, const TilesetVec& tilesetVec, int tileWidth, int rowCount, int columnCount);
		void ParseXMLAttribute(std::string attr);

	private:
		MapParser(){};
		std::map<std::string, TileMap*> m_Map;
		static MapParser* s_INSTANCE;


	};

}


#endif //WINTHER_ENGINE_MAPPARSER_H
