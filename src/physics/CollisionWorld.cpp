#include "physics/CollisionWorld.h"
#include "graphics/MapParser.h"

namespace Modwin
{


	bool CollisionWorld::BuildFromMap(const TileMap& map)
	{
		const auto& layers = map.GetLayers();

		const TileLayer* collisionLayer = nullptr;
		for (const auto& layer : layers)
		{
			if (layer.GetName() == "Collision")
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
		return false;
	}

	bool CollisionWorld::OverlapsSolidTiles(const Aabb& box) const noexcept
	{
		return false;
	}
}
