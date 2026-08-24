#ifndef MODWIN_ENGINE_KINEMATICMOVEMENT_H
#define MODWIN_ENGINE_KINEMATICMOVEMENT_H

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
		float deltaSeconds) noexcept;
}

#endif // MODWIN_ENGINE_KINEMATICMOVEMENT_H
