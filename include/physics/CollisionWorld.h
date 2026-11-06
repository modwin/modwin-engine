#ifndef MODWIN_ENGINE_COLLISIONWORLD_H
#define MODWIN_ENGINE_COLLISIONWORLD_H
#include "graphics/TileMap.h"

namespace Modwin
{
	class CollisionWorld
	{
		public:
			bool BuildFromMap(const TileMap& map);
			bool IsSolidTile(int column, int row) const noexcept;
			bool OverlapsSolidTiles(const Aabb& box) const noexcept;

		private:
			int m_Width = 0;
			int m_Height = 0;
			int m_TileWidth = 0;
			int m_TileHeight = 0;
			std::vector<std::uint8_t> m_SolidTiles;
	};
}


#endif //MODWIN_ENGINE_COLLISIONWORLD_H
