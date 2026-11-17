#ifndef MODWIN_ENGINE_GAMESTATE_H
#define MODWIN_ENGINE_GAMESTATE_H
#include "game/GameCamera.h"
#include "physics/CollisionWorld.h"

namespace Modwin
{
	struct GameState
	{
		std::size_t activeLayerIndex = 0;
		std::size_t activeTilesetIndex = 0;
		TileGid selectedTile = 0;

		CollisionWorld collisionWorld = CollisionWorld();
		int hoveredColumn = -1;
		int hoveredRow = -1;
		GameCamera camera;
	};
};



#endif //MODWIN_ENGINE_GAMESTATE_H
