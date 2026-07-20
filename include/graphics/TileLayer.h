//
// Created by komvu on 2025-01-04.
//

#ifndef WINTHER_ENGINE_TILELAYER_H
#define WINTHER_ENGINE_TILELAYER_H

#include <graphics/SurfaceLayer.h>
#include <utility>
#include <vector>
#include <string>

namespace Winther
{
	struct Tileset
	{
		Tileset();

		int firstId, lastId;
		int rowCount, columnCount;
		int tileWidth;
		int tileCount;
		std::string name, source;

	};

	//TileSetVec = TileSetList
	using TilesetVec = std::vector<Tileset>;
	// TileVec2D = TileMap
	using TileVec2D = std::vector<std::vector<int>>;

	class TileLayer : public SurfaceLayer
	{
	public:

		TileLayer(int tileWidth, int rowCount, int columnCount, TileVec2D tileVec, TilesetVec tileSetVec);

		void Render() override;
		void Update() override;

		int CalculateTileForCorruption(int row, int column);
		bool ContainsTile(int row, int tileId);
		inline TileVec2D GetTileVec(){return m_TileVec2D;}
		inline TilesetVec GetTileSetVec(){return m_Tilesets;}
	private:
		int m_TileWidth;
		int m_ColumnCount, m_RowCount;
		TileVec2D m_TileVec2D;
		TilesetVec m_Tilesets;
		std::vector<int> m_UsedTiles;
	};

} // Winther

#endif //WINTHER_ENGINE_TILELAYER_H
