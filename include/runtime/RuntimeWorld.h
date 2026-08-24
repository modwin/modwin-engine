#ifndef MODWIN_ENGINE_RUNTIMEWORLD_H
#define MODWIN_ENGINE_RUNTIMEWORLD_H

#include "component/Animation.h"
#include "component/KinematicBodyComponent.h"
#include "modwin/abi/ModwinGameApi.h"
#include "physics/Transform.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Modwin
{
	class RuntimeWorld
	{
	public:
		[[nodiscard]] ModwinEntityHandle CreateEntity(const ModwinEntityCreateInfo& createInfo);
		[[nodiscard]] bool DestroyEntity(ModwinEntityHandle handle) noexcept;
		[[nodiscard]] bool IsAlive(ModwinEntityHandle handle) const noexcept;

		[[nodiscard]] bool GetPosition(
			ModwinEntityHandle handle, Vector2D& position) const noexcept;
		[[nodiscard]] bool SetPosition(
			ModwinEntityHandle handle, Vector2D position) noexcept;
		[[nodiscard]] bool SetKinematicInput(
			ModwinEntityHandle handle, Vector2D direction) noexcept;
		[[nodiscard]] bool SetAnimation(
			ModwinEntityHandle handle,
			int row,
			int frameCount,
			int frameDurationMilliseconds);

		void FixedUpdate(float deltaSeconds) noexcept;
		void Update(float deltaSeconds) noexcept;
		void Render();
		void Clear() noexcept;

		[[nodiscard]] std::size_t GetEntityCount() const noexcept { return m_LiveEntityCount; }

	private:
		struct EntitySlot
		{
			std::uint32_t generation = 1;
			bool alive = false;
			Transform transform;
			Vector2D spriteSize{};
			std::string textureId;
			SDL_FlipMode flipMode = SDL_FLIP_NONE;
			std::optional<KinematicBodyComponent> kinematicBody;
			Vector2D desiredMovement{};
			std::optional<Animation> animation;
		};

		[[nodiscard]] EntitySlot* Resolve(ModwinEntityHandle handle) noexcept;
		[[nodiscard]] const EntitySlot* Resolve(ModwinEntityHandle handle) const noexcept;
		[[nodiscard]] static ModwinEntityHandle PackHandle(
			std::uint32_t index, std::uint32_t generation) noexcept;
		static void IncrementGeneration(std::uint32_t& generation) noexcept;

		std::vector<EntitySlot> m_Slots;
		std::vector<std::uint32_t> m_FreeIndices;
		std::size_t m_LiveEntityCount = 0;
	};
}

#endif // MODWIN_ENGINE_RUNTIMEWORLD_H
