#ifndef MODWIN_ENGINE_COLLISIONWORLD_H
#define MODWIN_ENGINE_COLLISIONWORLD_H
#include "physics/Vector2D.h"
#include "graphics/TileMap.h"
#include <cstdint>
#include <vector>

namespace Modwin
{
	struct Aabb
	{
		Vector2D minimum;
		Vector2D maximum;
	};

	struct CollisionShape
	{
		Vector2D offset;
		Vector2D size;
	};

	class CollisionWorld
	{
		public:
			bool BuildFromMap(const TileMap& map);
			[[nodiscard]] bool IsSolidTile(int column, int row) const noexcept;
			[[nodiscard]] bool OverlapsSolidTiles(const Aabb& box) const noexcept;

		private:
			int m_Width = 0;
			int m_Height = 0;
			int m_TileWidth = 0;
			int m_TileHeight = 0;
			std::vector<std::uint8_t> m_SolidTiles;
	};
}


#endif //MODWIN_ENGINE_COLLISIONWORLD_H
