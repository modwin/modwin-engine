//
// Created by komvu on 2025-01-04.
//

#include "graphics/TileLayer.h"
#include <graphics/TextureManager.h>

#include <utility>
#include <algorithm>

namespace Modwin
{
	TileLayer::TileLayer(int tileWidth, int rowCount, int columnCount, TileVec2D tileVec, TilesetVec tileSetVec)
			: SurfaceLayer()
	{
		m_TileWidth = tileWidth;
		m_RowCount = rowCount;
		m_ColumnCount = columnCount;
		m_TileVec2D = std::move(tileVec);
		m_Tilesets = std::move(tileSetVec);

		for (int i = 0; i < m_Tilesets.size(); i++)
		{
			m_UsedTiles.push_back(m_Tilesets[i].firstId);
			TextureManager::GetInstance()->Load(m_Tilesets[i].name.c_str(), "tiles/");
		}
	}

	Tileset::Tileset()
	{

	}

	void TileLayer::Render()
	{
		for (unsigned int row = 0; row < m_RowCount; row++)
		{
			for (unsigned int column = 0; column < m_ColumnCount; column++)
			{
				int tileId = m_TileVec2D[row][column];

				// Skip empty tiles


				if(tileId == 0)
				{
					continue;
				}

				// Determine the correct tileset for the tileId
				int tilesetIndex = -1;

				for (unsigned int k = 0; k < m_Tilesets.size(); k++)
				{
					if (tileId >= m_Tilesets[k].firstId && tileId < m_Tilesets[k].firstId + m_Tilesets[k].tileCount)
					{
						tilesetIndex = k;
						break;
					}
				}


				if (tilesetIndex == -1)		// If no matching tileset is found, skip rendering
				{
					continue;
				}

				// Calculate the tile's position in the tileset
				Tileset tileset = m_Tilesets[tilesetIndex];
				int localTileId = tileId - tileset.firstId; 		// Normalize tileId to tileset range
				int tileRow = localTileId / tileset.columnCount;
				int tileColumn = localTileId % tileset.columnCount;

				// Render the tile
//				Properties p(SDL_FLIP_NONE, column * tileset.tileWidth, row * tileset.tileWidth, tileset.tileWidth, tileset.tileWidth, tileset.name, tileset.source);
//				EntityManager::GetInstance()->addEntity(tileset.name, p);
				TextureManager::GetInstance()->DrawTile(
						tileset.name, tileset.tileWidth, column * tileset.tileWidth, row * tileset.tileWidth, tileRow, tileColumn);
			}
		}
	}

	void TileLayer::Update()
	{

	}

	bool TileLayer::ContainsTile(int row, int tileId)
	{
		for (int usedTile : m_UsedTiles)
		{
			for (auto tile : m_TileVec2D[row])
			{
				if (tileId == tileId - tile) return true;
			}
		}
		return false;
	}

	int TileLayer::CalculateTileForCorruption(int row, int column)
	{
		// Check bounds to avoid out-of-bounds access
		auto isValidIndex = [this](int r, int c) {
			return r >= 0 && r < static_cast<int>(m_RowCount) && c >= 0 && c < static_cast<int>(m_ColumnCount);
		};

		// Check if a tileId is valid
		auto isValidTileId = [](int tileId) {
			return tileId > 1 && tileId < 219; // Adjust range as needed
		};

		// Default fallback tile
		const int defaultTileId = 1; // Replace with a tileId that represents a generic or placeholder tile

		// Search for the nearest valid neighbor
		for (int radius = 1; radius <= 2; radius++) // Search within a 2-tile radius
		{
			for (int dr = -radius; dr <= radius; dr++)
			{
				for (int dc = -radius; dc <= radius; dc++)
				{
					int neighborRow = row + dr;
					int neighborCol = column + dc;

					if (isValidIndex(neighborRow, neighborCol))
					{
						int neighborTileId = m_TileVec2D[neighborRow][neighborCol];
						if (isValidTileId(neighborTileId))
						{
							return neighborTileId; // Use the first valid neighboring tile found
						}
					}
				}
			}
		}

		// If no valid neighbor is found, return the default tile
		return defaultTileId;
	}
	
//	bool TileLayer::VerifyTileRowdata(){
//
//	}


} // Modwin