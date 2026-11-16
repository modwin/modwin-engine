#include "physics/CollisionWorld.h"
#include "graphics/MapParser.h"
#include "cmath"

namespace Modwin
{


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
		return false;
	}
}
