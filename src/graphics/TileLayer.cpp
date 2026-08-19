#include "graphics/TileLayer.h"

#include "graphics/TextureManager.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace Modwin
{
	TileLayer::TileLayer(
		std::string name,
		const int width,
		const int height,
		const int tileWidth,
		std::vector<TileGid> tiles)
		: m_Name(std::move(name)),
		  m_TileWidth(tileWidth),
		  m_Width(width),
		  m_Height(height),
		  m_Tiles(std::move(tiles))
	{
		if (m_Width <= 0 || m_Height <= 0 || m_TileWidth <= 0)
		{
			throw std::invalid_argument("TileLayer dimensions must be positive.");
		}

		const auto expectedSize =
			static_cast<std::size_t>(m_Width) * static_cast<std::size_t>(m_Height);
		if (m_Tiles.size() != expectedSize)
		{
			throw std::invalid_argument("TileLayer tile count does not match its dimensions.");
		}
	}

	void TileLayer::Render(
		const TilesetVec& tilesets, const float cameraX, const float cameraY) const
	{
		for (int row = 0; row < m_Height; ++row)
		{
			for (int column = 0; column < m_Width; ++column)
			{
				const TileGid rawGid = GetTile(column, row);
				const TileGid tileId = rawGid & TileIdMask;
				if (tileId == 0)
				{
					continue;
				}

				const auto tileset = std::find_if(
					tilesets.begin(), tilesets.end(),
					[tileId](const Tileset& candidate)
					{
						return tileId >= candidate.firstId && tileId <= candidate.lastId;
					});

				if (tileset == tilesets.end() || tileset->columnCount <= 0)
				{
					continue;
				}

				const TileGid localTileId = tileId - tileset->firstId;
				const int sourceRow = static_cast<int>(localTileId) / tileset->columnCount;
				const int sourceColumn = static_cast<int>(localTileId) % tileset->columnCount;

				int flipFlags = SDL_FLIP_NONE;
				if ((rawGid & FlipHorizontal) != 0)
				{
					flipFlags |= SDL_FLIP_HORIZONTAL;
				}
				if ((rawGid & FlipVertical) != 0)
				{
					flipFlags |= SDL_FLIP_VERTICAL;
				}

				// A diagonal Tiled flip also requires a 90-degree rotation. The raw
				// flag remains in m_Tiles so it can be preserved when the map is saved.
				TextureManager::GetInstance()->DrawTile(
					tileset->name,
					m_TileWidth,
					static_cast<float>(column * m_TileWidth) - cameraX,
					static_cast<float>(row * m_TileWidth) - cameraY,
					sourceRow,
					sourceColumn,
					static_cast<SDL_FlipMode>(flipFlags));
			}
		}
	}

	void TileLayer::Update()
	{
	}

	bool TileLayer::IsInBounds(const int column, const int row) const noexcept
	{
		return column >= 0 && column < m_Width && row >= 0 && row < m_Height;
	}

	TileGid TileLayer::GetTile(const int column, const int row) const
	{
		if (!IsInBounds(column, row))
		{
			throw std::out_of_range("Tile coordinates are outside the layer.");
		}

		return m_Tiles[GetIndex(column, row)];
	}

	bool TileLayer::SetTile(const int column, const int row, const TileGid tileGid) noexcept
	{
		if (!IsInBounds(column, row))
		{
			return false;
		}

		TileGid& currentTile = m_Tiles[GetIndex(column, row)];
		if (currentTile == tileGid)
		{
			return false;
		}

		currentTile = tileGid;
		return true;
	}

	bool TileLayer::ContainsTile(const int row, const TileGid tileId) const noexcept
	{
		if (row < 0 || row >= m_Height)
		{
			return false;
		}

		const auto first = m_Tiles.begin() + static_cast<std::ptrdiff_t>(GetIndex(0, row));
		const auto last = first + m_Width;
		return std::find(first, last, tileId) != last;
	}

	int TileLayer::GetWidth() const noexcept
	{
		return m_Width;
	}

	int TileLayer::GetHeight() const noexcept
	{
		return m_Height;
	}

	int TileLayer::GetTileWidth() const noexcept
	{
		return m_TileWidth;
	}

	const std::string& TileLayer::GetName() const noexcept
	{
		return m_Name;
	}

	const std::vector<TileGid>& TileLayer::GetTiles() const noexcept
	{
		return m_Tiles;
	}

	std::size_t TileLayer::GetIndex(const int column, const int row) const noexcept
	{
		return static_cast<std::size_t>(row) * static_cast<std::size_t>(m_Width) +
			static_cast<std::size_t>(column);
	}
}
