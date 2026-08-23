#ifndef MODWIN_ENGINE_RIGIDBODY_H
#define MODWIN_ENGINE_RIGIDBODY_H

#include "physics/Vector2D.h"

namespace Modwin
{
	// A dynamic-body primitive. Transform remains the authoritative world position;
	// Integrate returns the displacement that its owning physics system should apply.
	class RigidBody
	{
	public:
		explicit RigidBody(float mass = 1.0F);

		void SetMass(float mass);
		[[nodiscard]] float GetMass() const noexcept { return m_Mass; }

		void SetVelocity(Vector2D velocity) noexcept { m_Velocity = velocity; }
		[[nodiscard]] const Vector2D& GetVelocity() const noexcept { return m_Velocity; }
		[[nodiscard]] const Vector2D& GetAcceleration() const noexcept { return m_Acceleration; }

		void ApplyForce(Vector2D force) noexcept { m_AccumulatedForce += force; }
		void ClearForces() noexcept { m_AccumulatedForce = {}; }

		[[nodiscard]] Vector2D Integrate(
			float deltaSeconds, Vector2D externalAcceleration = {});

	private:
		float m_Mass = 1.0F;
		Vector2D m_AccumulatedForce{};
		Vector2D m_Velocity{};
		Vector2D m_Acceleration{};
	};
}

#endif // MODWIN_ENGINE_RIGIDBODY_H
