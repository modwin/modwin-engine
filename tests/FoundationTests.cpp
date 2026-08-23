#include "component/KinematicBodyComponent.h"
#include "entity/EntityManager.h"
#include "game/InputBindings.h"
#include "physics/KinematicMovement.h"
#include "physics/RigidBody.h"
#include "physics/Transform.h"
#include "physics/Vector2D.h"

#include <cmath>
#include <iostream>
#include <string>

namespace
{
	bool Check(const bool condition, const std::string& message)
	{
		if (!condition)
		{
			std::cerr << "FAILED: " << message << '\n';
		}
		return condition;
	}

	bool NearlyEqual(const float left, const float right, const float epsilon = 0.001F)
	{
		return std::abs(left - right) <= epsilon;
	}

	Modwin::Vector2D SimulateMovement(const float deltaSeconds, const int steps)
	{
		Modwin::KinematicBodyComponent body;
		body.maximumSpeed = 180.0F;
		body.acceleration = 100'000.0F;
		Modwin::Transform transform;
		for (int step = 0; step < steps; ++step)
		{
			Modwin::UpdateKinematicMovement(body, transform, {1.0F, 0.0F}, deltaSeconds);
		}
		return transform.GetPosition();
	}
}

int main()
{
	bool passed = true;

	Modwin::Vector2D vector{3.0F, 4.0F};
	passed &= Check(NearlyEqual(vector.Length(), 5.0F), "Vector length should use Pythagoras.");
	passed &= Check(
		vector.Normalized().NearlyEquals({0.6F, 0.8F}),
		"A normalized vector should retain direction and have unit length.");
	passed &= Check(
		Modwin::Vector2D{1.0F, 2.0F} != Modwin::Vector2D{1.0F, 3.0F},
		"Vectors should be unequal when either component differs.");

	Modwin::RigidBody rigidBody{2.0F};
	rigidBody.ApplyForce({10.0F, 0.0F});
	const Modwin::Vector2D firstDisplacement = rigidBody.Integrate(0.5F);
	passed &= Check(
		rigidBody.GetVelocity().NearlyEquals({2.5F, 0.0F}) &&
		firstDisplacement.NearlyEquals({1.25F, 0.0F}),
		"RigidBody should accumulate velocity and return displacement in seconds.");
	const Modwin::Vector2D secondDisplacement = rigidBody.Integrate(0.5F);
	passed &= Check(
		secondDisplacement.NearlyEquals({1.25F, 0.0F}),
		"Forces should clear after integration while velocity persists.");

	Modwin::KinematicBodyComponent diagonalBody;
	diagonalBody.acceleration = 100'000.0F;
	Modwin::Transform diagonalTransform;
	Modwin::UpdateKinematicMovement(
		diagonalBody, diagonalTransform, {1.0F, 1.0F}, 1.0F / 60.0F);
	passed &= Check(
		NearlyEqual(diagonalBody.velocity.Length(), diagonalBody.maximumSpeed),
		"Diagonal movement should be normalized to maximum speed.");

	const Modwin::Vector2D sixtyHzPosition = SimulateMovement(1.0F / 60.0F, 60);
	const Modwin::Vector2D oneTwentyHzPosition = SimulateMovement(1.0F / 120.0F, 120);
	passed &= Check(
		sixtyHzPosition.NearlyEquals(oneTwentyHzPosition, 0.01F),
		"Movement distance should not depend on render/update frequency.");

	Modwin::InputBindings bindings;
	bindings.Set(Modwin::GameAction::MoveLeft, SDL_SCANCODE_LEFT);
	passed &= Check(
		bindings.Get(Modwin::GameAction::MoveLeft) == SDL_SCANCODE_LEFT,
		"Changing a binding should replace its previous key.");

	Modwin::EntityManager entities;
	Modwin::EntityDescriptor descriptor;
	auto& firstEntity = entities.CreateEntity("test", descriptor);
	passed &= Check(firstEntity.GetId() == 1, "The first runtime entity should receive ID 1.");
	passed &= Check(
		entities.FindByTag("test").size() == 1,
		"Entity lookup should return entities with a matching tag.");
	entities.Clear();
	auto& resetEntity = entities.CreateEntity("test", descriptor);
	passed &= Check(
		resetEntity.GetId() == 1,
		"Clearing a runtime world should reset its local entity ID sequence.");

	return passed ? 0 : 1;
}
