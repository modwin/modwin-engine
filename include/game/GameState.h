#ifndef MODWIN_ENGINE_GAMESTATE_H
#define MODWIN_ENGINE_GAMESTATE_H
#include "game/GameCamera.h"
#include "physics/CollisionWorld.h"

namespace Modwin
{
	struct GameState
	{
		CollisionWorld collisionWorld = CollisionWorld();
		GameCamera camera;
	};
};



#endif //MODWIN_ENGINE_GAMESTATE_H
