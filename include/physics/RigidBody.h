//
// Created by komvu on 2025-01-02.
//

#ifndef WINTHER_ENGINE_RIGIDBODY_H
#define WINTHER_ENGINE_RIGIDBODY_H

#include <physics/Vector2D.h>

#define MASS_MULTIPLE 1.0f
#define GRAVITATIONAL_FORCE 9.8

namespace Winther{

	class RigidBody
	{
	public:
		RigidBody(): m_Force(0, 0), m_Friction(0, 0), m_Position(0, 0), m_Velocity(0, 0)
		{
			m_Mass = MASS_MULTIPLE;
			m_Gravity = GRAVITATIONAL_FORCE;

		};
		virtual ~RigidBody();

		inline void SetMass(float mass){m_Mass = mass;}
		inline void SetGravity(float gravity){m_Gravity = gravity;}

		inline void ApplyForce(const Vector2D& force){m_Force = force;}
		inline void ApplyForceX(float forceX){m_Force.X=forceX;}
		inline void ApplyForceY(float forceY){m_Force.Y=forceY;}
		inline void ResetForce(){m_Force = Vector2D(0, 0);}

		inline void ApplyFriction(const Vector2D& friction){m_Friction = friction;}
		inline void ResetFriction(){m_Friction = Vector2D(0, 0);}

		inline void Update(float deltaTime)
		{
			m_Acceleration.X = (m_Force.X + m_Friction.X) / m_Mass;
			m_Acceleration.Y = m_Gravity + m_Force.Y / m_Mass;
			m_Velocity = m_Acceleration * deltaTime;
			m_Position = m_Velocity * deltaTime;
		}
	private:
		float m_Mass;
		float m_Gravity;

		Vector2D m_Force;
		Vector2D m_Friction;

		Vector2D m_Position;
		Vector2D m_Velocity;
		Vector2D m_Acceleration;

	};
}



#endif //WINTHER_ENGINE_RIGIDBODY_H
