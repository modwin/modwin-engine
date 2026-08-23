#include "component/KinematicBodyComponent.h"
#include "game/InputBindings.h"
#include "modwin/abi/ModwinGameApi.h"
#include "physics/KinematicMovement.h"
#include "physics/RigidBody.h"
#include "physics/Transform.h"
#include "physics/Vector2D.h"
#include "runtime/RuntimeWorld.h"

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
	bindings.Set(Modwin::GameAction::MoveLeft, MODWIN_KEY_D);
	passed &= Check(
		bindings.Get(Modwin::GameAction::MoveLeft) == MODWIN_KEY_D,
		"Changing a binding should replace its previous key.");

	ModwinEntityCreateInfo createInfo{};
	createInfo.struct_size = static_cast<uint32_t>(sizeof(ModwinEntityCreateInfo));
	createInfo.position = {4.0F, 8.0F};
	createInfo.sprite_size = {16.0F, 16.0F};
	createInfo.texture_id = "test";
	Modwin::RuntimeWorld runtimeWorld;
	const ModwinEntityHandle firstEntity = runtimeWorld.CreateEntity(createInfo);
	passed &= Check(
		firstEntity != MODWIN_INVALID_ENTITY_HANDLE && runtimeWorld.IsAlive(firstEntity),
		"Creating a runtime entity should return a live opaque handle.");
	Modwin::Vector2D entityPosition;
	passed &= Check(
		runtimeWorld.GetPosition(firstEntity, entityPosition) &&
		entityPosition.NearlyEquals({4.0F, 8.0F}),
		"Runtime entity state should be accessible through its handle.");
	runtimeWorld.Clear();
	passed &= Check(
		!runtimeWorld.IsAlive(firstEntity),
		"Clearing a runtime world should invalidate existing handles.");
	const ModwinEntityHandle replacementEntity = runtimeWorld.CreateEntity(createInfo);
	passed &= Check(
		replacementEntity != firstEntity && runtimeWorld.IsAlive(replacementEntity),
		"Reused entity slots should receive a new generation handle.");

	return passed ? 0 : 1;
}
