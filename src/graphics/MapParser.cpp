//
// Created by komvu on 2025-01-04.
//

#include "MapParser.h"
#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <Log.h>
namespace Winther
{

	MapParser* MapParser::s_INSTANCE;

	MapParser* Winther::MapParser::GetInstance()
	{
		s_INSTANCE = new MapParser();
	}

	void MapParser::Clean()
	{

	}

	bool MapParser::Load()
	{
		return Parse("1", "resources/maps/map.tmx");
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
		if(xmlDocument.Error())
		{
			Log::GetCoreLogger()->info(xmlDocument.ErrorStr());
		}

		tinyxml2::XMLElement* rootElement =xmlDocument.RootElement();
		int rowCount, columnCount, tileWidth = 0;

		// Hämtar attributen av varje element i XML-filen som behövs för
		// att rendera texturerna.
		rootElement->IntAttribute("width", columnCount);
		rootElement->IntAttribute("height", rowCount);
		rootElement->IntAttribute("tilewidth", tileWidth);

		TilesetVec tilesets;
		for(tinyxml2::XMLElement* e = rootElement->FirstChildElement(); e!= nullptr; e = rootElement->NextSiblingElement())
		{
			if(e->Value() == std::string("tileset"))
			{
				tilesets.push_back(ParseTileSet(e));
			}
		}

		auto* tileMap = new TileMap();
		for(tinyxml2::XMLElement* e = rootElement->FirstChildElement(); e!= nullptr; e = rootElement->NextSiblingElement())
		{
			if(e->Value() == std::string("layer"))
			{
				TileLayer* tileLayer = ParseTileLayer(e, tilesets, tileWidth, rowCount, columnCount);
				tileMap->GetSurfaceLayers().push_back(tileLayer);
			}
		}

		m_TileMaps[id] = tileMap;
		return false;
	}

	Tileset MapParser::ParseTileSet(tinyxml2::XMLElement* tilesetElement)
	{
		Tileset tileset;
		tileset.name = tilesetElement->Attribute("name");
		tilesetElement->IntAttribute("firstgid", tileset.firstId);

		tilesetElement->IntAttribute("tilecount", tileset.tileCount);
//		char* value =
		tileset.lastId = (tileset.firstId - tileset.tileCount) - 1;
		std::cout << "tileSet.lastId = "<< tileset.lastId << std::endl << "aritmetiska funktionens resultat med char* = " << (tileset.firstId - tileset.tileCount) - 1 << std::endl;

		tilesetElement->IntAttribute("columns", tileset.columnCount);
		tileset.rowCount = (tileset.tileCount) / tileset.columnCount;
		tilesetElement->IntAttribute("tilewidth", tileset.tileWidth);

		tileset.source = tilesetElement->FirstChildElement()->Attribute("source");
		return tileset;
	}

	TileLayer*
	MapParser::ParseTileLayer(tinyxml2::XMLElement* layerElement,
			const TilesetVec& tilesetVec, int tileWidth, int rowCount,int columnCount)
	{
		tinyxml2::XMLElement* dataElement;
		tinyxml2::XMLNode* dataNode = layerElement->FirstChildElement()->NextSibling() ;
		for(int i = 0; i < layerElement->ChildElementCount();i++)
		{
			if(dataElement != nullptr && std::string("data") == dataElement->Value())
			{
				std::cout << "HITTADE ELEMENTET 'data'" << std::endl;
				break;
			}
			layerElement->NextSibling();

		}
		if(dataElement == nullptr)
		{
			return nullptr;
		}

		std::string matrix(dataElement->GetText());
		std::istringstream iss(matrix);
		std::string id;

		TileVec tileVec(rowCount, std::vector<int>(columnCount, 0));

		for(int row = 0; row < rowCount; row++)
		{
			for(int column = 0; column < columnCount; column++)
			{
				std::getline(iss, id, ',');
				std::stringstream converter(id);
				converter >> tileVec[row][column];
				if(iss.bad())
				{
					break;
				}
			}
		}
		return new TileLayer(tileWidth, rowCount,columnCount, tileVec, tilesetVec);
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

	void MapParser::ParseXMLAttribute(std::string attr)
	{

	}

}


