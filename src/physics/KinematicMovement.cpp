#include "physics/KinematicMovement.h"

#include <algorithm>
#include <physics/CollisionWorld.h>

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
		const CollisionWorld& collisionWorld,
		const CollisionShape& shape,
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
		// body.velocity = MoveTowards(body.velocity, targetVelocity, rate * deltaSeconds);
		// transform.Translate(body.velocity * deltaSeconds);

		body.velocity = MoveTowards(
			body.velocity,
			targetVelocity,
			rate * deltaSeconds);

		const Vector2D delta = body.velocity * deltaSeconds;
		Vector2D position = transform.GetPosition();

		Vector2D candidate = position;
		candidate.X += delta.X;


		if (collisionWorld.OverlapsSolidTiles(
				Aabb::MakeWorldAabb(candidate, shape)))
		{
			body.velocity.X = 0.0F;
		}
		else
		{
			position.X = candidate.X;
		}

		candidate = position;
		candidate.Y += delta.Y;

		if (collisionWorld.OverlapsSolidTiles(
				Aabb::MakeWorldAabb(candidate, shape)))
		{
			body.velocity.Y = 0.0F;
		}
		else
		{
			position.Y = candidate.Y;
		}

		transform.SetPosition(position);
	}
}
