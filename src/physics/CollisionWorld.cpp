#include "physics/CollisionWorld.h"
#include "graphics/MapParser.h"
#include <cmath>

namespace Modwin
{
	Aabb Aabb::MakeWorldAabb(Vector2D position, const CollisionShape& shape) noexcept
	{
		const Vector2D min = position + shape.offset;
		return {min, min + shape.size};
	}

	bool CollisionWorld::BuildFromMap(const TileMap& map)
	{
		const auto& layers = map.GetLayers();

		const TileLayer* collisionLayer = nullptr;
		for (const auto& layer : layers)
		{
			if (layer.IsCollisionLayer())
			{
				collisionLayer = &layer;
				break;
			}
		}

		if (collisionLayer == nullptr)
		{
			return false;
		}

		m_Width = collisionLayer->GetWidth();
		m_Height = collisionLayer->GetHeight();
		m_TileWidth = map.GetTileWidth();
		m_TileHeight = map.GetTileHeight();

		const auto& gids = collisionLayer->GetTiles();
		m_SolidTiles.resize(gids.size());

		for (std::size_t i = 0; i < gids.size(); ++i)
		{
			m_SolidTiles[i] = gids[i] != 0 ? 1U : 0U;
		}

		return true;
	}

	bool CollisionWorld::IsSolidTile(int column, int row) const noexcept
	{
		if (column < 0 || column >= m_Width ||
			row < 0 || row >= m_Height)
		{
			return true;
		}

		const std::size_t index =
			static_cast<std::size_t>(row) *
			static_cast<std::size_t>(m_Width) +
			static_cast<std::size_t>(column);

		return m_SolidTiles[index] != 0;
	}

	bool CollisionWorld::OverlapsSolidTiles(const Aabb& box) const noexcept
	{
		const int firstColumn =
	static_cast<int>(std::floor(box.minimum.X / m_TileWidth));
		const int lastColumn =
			static_cast<int>(std::ceil(box.maximum.X / m_TileWidth)) - 1;

		const int firstRow =
			static_cast<int>(std::floor(box.minimum.Y / m_TileHeight));
		const int lastRow =
			static_cast<int>(std::ceil(box.maximum.Y / m_TileHeight)) - 1;

		for (int row = firstRow; row <= lastRow; ++row)
		{
			for (int column = firstColumn; column <= lastColumn; ++column)
			{
				if (IsSolidTile(column, row))
				{
					return true;
				}
			}
		}
		return false;
	}
}
