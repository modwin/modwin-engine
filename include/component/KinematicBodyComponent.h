#ifndef MODWIN_ENGINE_KINEMATICBODYCOMPONENT_H
#define MODWIN_ENGINE_KINEMATICBODYCOMPONENT_H

#include "physics/Vector2D.h"

namespace Modwin
{
	struct KinematicBodyComponent
	{
		Vector2D velocity{};
		float maximumSpeed = 180.0F;
		float acceleration = 1200.0F;
		float deceleration = 1600.0F;
	};
}

#endif // MODWIN_ENGINE_KINEMATICBODYCOMPONENT_H
