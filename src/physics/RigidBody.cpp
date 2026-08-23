#include "physics/RigidBody.h"

#include <stdexcept>

namespace Modwin
{
	RigidBody::RigidBody(const float mass)
	{
		SetMass(mass);
	}

	void RigidBody::SetMass(const float mass)
	{
		if (mass <= 0.0F)
		{
			throw std::invalid_argument("RigidBody mass must be greater than zero.");
		}

		m_Mass = mass;
	}

	Vector2D RigidBody::Integrate(
		const float deltaSeconds, const Vector2D externalAcceleration)
	{
		if (deltaSeconds <= 0.0F)
		{
			return {};
		}

		m_Acceleration = m_AccumulatedForce / m_Mass + externalAcceleration;
		m_Velocity += m_Acceleration * deltaSeconds;
		ClearForces();
		return m_Velocity * deltaSeconds;
	}
}
