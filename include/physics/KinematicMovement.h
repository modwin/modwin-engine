#ifndef MODWIN_ENGINE_KINEMATICMOVEMENT_H
#define MODWIN_ENGINE_KINEMATICMOVEMENT_H

#include "CollisionWorld.h"
#include "component/KinematicBodyComponent.h"
#include "physics/Transform.h"

namespace Modwin
{
	[[nodiscard]] Vector2D MoveTowards(
		Vector2D current, Vector2D target, float maximumDelta) noexcept;

	void UpdateKinematicMovement(
		KinematicBodyComponent& body,
		Transform& transform,
		Vector2D desiredDirection,
		const CollisionWorld& collisionWorld,
		const CollisionShape& shape,
		float deltaSeconds) noexcept;
}

#endif // MODWIN_ENGINE_KINEMATICMOVEMENT_H
