#include "physics/KinematicMovement.h"

#include <algorithm>

namespace Modwin
{
	Vector2D MoveTowards(
		const Vector2D current, const Vector2D target, const float maximumDelta) noexcept
	{
		const Vector2D difference = target - current;
		const float distance = difference.Length();
		if (distance <= maximumDelta || distance == 0.0F)
		{
			return target;
		}

		return current + difference * (maximumDelta / distance);
	}

	void UpdateKinematicMovement(
		KinematicBodyComponent& body,
		Transform& transform,
		Vector2D desiredDirection,
		const float deltaSeconds) noexcept
	{
		if (deltaSeconds <= 0.0F)
		{
			return;
		}

		body.maximumSpeed = std::max(0.0F, body.maximumSpeed);
		body.acceleration = std::max(0.0F, body.acceleration);
		body.deceleration = std::max(0.0F, body.deceleration);

		if (desiredDirection.LengthSquared() > 1.0F)
		{
			desiredDirection.Normalize();
		}

		const bool hasMovementInput = desiredDirection.LengthSquared() > 0.0F;
		const Vector2D targetVelocity = desiredDirection * body.maximumSpeed;
		const float rate = hasMovementInput ? body.acceleration : body.deceleration;
		body.velocity = MoveTowards(body.velocity, targetVelocity, rate * deltaSeconds);
		transform.Translate(body.velocity * deltaSeconds);
	}
}
