#include "runtime/RuntimeWorld.h"

#include "graphics/TextureManager.h"
#include "physics/KinematicMovement.h"

#include <limits>
#include <utility>

namespace Modwin
{
	ModwinEntityHandle RuntimeWorld::CreateEntity(const ModwinEntityCreateInfo& createInfo)
	{
		std::string textureId = createInfo.texture_id != nullptr ? createInfo.texture_id : "";
		std::uint32_t index = 0;
		if (!m_FreeIndices.empty())
		{
			index = m_FreeIndices.back();
			m_FreeIndices.pop_back();
		}
		else
		{
			if (m_Slots.size() >= std::numeric_limits<std::uint32_t>::max())
			{
				return MODWIN_INVALID_ENTITY_HANDLE;
			}
			index = static_cast<std::uint32_t>(m_Slots.size());
			m_Slots.emplace_back();
		}

		EntitySlot& slot = m_Slots[index];
		slot.transform.SetPosition({createInfo.position.x, createInfo.position.y});
		slot.spriteSize = {createInfo.sprite_size.x, createInfo.sprite_size.y};
		slot.textureId = std::move(textureId);
		slot.flipMode = createInfo.flip_mode == MODWIN_FLIP_HORIZONTAL
			? SDL_FLIP_HORIZONTAL
			: createInfo.flip_mode == MODWIN_FLIP_VERTICAL
				? SDL_FLIP_VERTICAL
				: createInfo.flip_mode ==
					(MODWIN_FLIP_HORIZONTAL | MODWIN_FLIP_VERTICAL)
					? static_cast<SDL_FlipMode>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL)
					: SDL_FLIP_NONE;
		slot.desiredMovement = {};
		slot.animation.reset();

		if ((createInfo.flags & MODWIN_ENTITY_HAS_KINEMATIC_BODY) != 0U)
		{
			slot.kinematicBody.emplace();
			slot.kinematicBody->maximumSpeed = createInfo.maximum_speed;
			slot.kinematicBody->acceleration = createInfo.acceleration;
			slot.kinematicBody->deceleration = createInfo.deceleration;
		}
		else
		{
			slot.kinematicBody.reset();
		}

		slot.alive = true;
		++m_LiveEntityCount;
		return PackHandle(index, slot.generation);
	}

	bool RuntimeWorld::DestroyEntity(const ModwinEntityHandle handle) noexcept
	{
		EntitySlot* slot = Resolve(handle);
		if (slot == nullptr)
		{
			return false;
		}

		const std::uint32_t index = static_cast<std::uint32_t>((handle & UINT64_C(0xFFFFFFFF)) - 1U);
		slot->alive = false;
		slot->textureId.clear();
		slot->kinematicBody.reset();
		slot->animation.reset();
		IncrementGeneration(slot->generation);
		m_FreeIndices.push_back(index);
		--m_LiveEntityCount;
		return true;
	}

	bool RuntimeWorld::IsAlive(const ModwinEntityHandle handle) const noexcept
	{
		return Resolve(handle) != nullptr;
	}

	bool RuntimeWorld::GetPosition(
		const ModwinEntityHandle handle, Vector2D& position) const noexcept
	{
		const EntitySlot* slot = Resolve(handle);
		if (slot == nullptr)
		{
			return false;
		}
		position = slot->transform.GetPosition();
		return true;
	}

	bool RuntimeWorld::SetPosition(
		const ModwinEntityHandle handle, const Vector2D position) noexcept
	{
		EntitySlot* slot = Resolve(handle);
		if (slot == nullptr)
		{
			return false;
		}
		slot->transform.SetPosition(position);
		return true;
	}

	bool RuntimeWorld::SetKinematicInput(
		const ModwinEntityHandle handle, const Vector2D direction) noexcept
	{
		EntitySlot* slot = Resolve(handle);
		if (slot == nullptr || !slot->kinematicBody.has_value())
		{
			return false;
		}
		slot->desiredMovement = direction;
		return true;
	}

	bool RuntimeWorld::SetAnimation(
		const ModwinEntityHandle handle,
		const int row,
		const int frameCount,
		const int frameDurationMilliseconds)
	{
		EntitySlot* slot = Resolve(handle);
		if (slot == nullptr)
		{
			return false;
		}
		if (!slot->animation.has_value())
		{
			slot->animation.emplace();
		}
		slot->animation->SetValues(
			slot->textureId,
			row,
			frameCount,
			frameDurationMilliseconds,
			slot->flipMode);
		return true;
	}

	void RuntimeWorld::FixedUpdate(const float deltaSeconds) noexcept
	{
		for (EntitySlot& slot : m_Slots)
		{
			if (slot.alive && slot.kinematicBody.has_value())
			{
				UpdateKinematicMovement(
					*slot.kinematicBody,
					slot.transform,
					slot.desiredMovement,
					deltaSeconds);
			}
		}
	}

	void RuntimeWorld::Update(const float deltaSeconds) noexcept
	{
		for (EntitySlot& slot : m_Slots)
		{
			if (slot.alive && slot.animation.has_value())
			{
				slot.animation->Update(deltaSeconds);
			}
		}
	}

	void RuntimeWorld::Render()
	{
		for (EntitySlot& slot : m_Slots)
		{
			if (!slot.alive)
			{
				continue;
			}
			if (slot.animation.has_value())
			{
				slot.animation->Draw(
					slot.transform.GetX(),
					slot.transform.GetY(),
					slot.spriteSize.X,
					slot.spriteSize.Y);
			}
			else
			{
				TextureManager::GetInstance()->Draw(
					slot.textureId,
					slot.transform.GetX(),
					slot.transform.GetY(),
					slot.spriteSize.X,
					slot.spriteSize.Y,
					slot.flipMode);
			}
		}
	}

	void RuntimeWorld::Clear() noexcept
	{
		m_FreeIndices.clear();
		for (std::uint32_t index = 0; index < m_Slots.size(); ++index)
		{
			EntitySlot& slot = m_Slots[index];
			if (slot.alive)
			{
				slot.alive = false;
				IncrementGeneration(slot.generation);
			}
			slot.textureId.clear();
			slot.kinematicBody.reset();
			slot.animation.reset();
			m_FreeIndices.push_back(index);
		}
		m_LiveEntityCount = 0;
	}

	RuntimeWorld::EntitySlot* RuntimeWorld::Resolve(const ModwinEntityHandle handle) noexcept
	{
		return const_cast<EntitySlot*>(static_cast<const RuntimeWorld*>(this)->Resolve(handle));
	}

	const RuntimeWorld::EntitySlot* RuntimeWorld::Resolve(
		const ModwinEntityHandle handle) const noexcept
	{
		if (handle == MODWIN_INVALID_ENTITY_HANDLE)
		{
			return nullptr;
		}
		const std::uint64_t encodedIndex = handle & UINT64_C(0xFFFFFFFF);
		if (encodedIndex == 0U)
		{
			return nullptr;
		}
		const auto index = static_cast<std::uint32_t>(encodedIndex - 1U);
		const auto generation = static_cast<std::uint32_t>(handle >> 32U);
		if (index >= m_Slots.size())
		{
			return nullptr;
		}
		const EntitySlot& slot = m_Slots[index];
		return slot.alive && slot.generation == generation ? &slot : nullptr;
	}

	ModwinEntityHandle RuntimeWorld::PackHandle(
		const std::uint32_t index, const std::uint32_t generation) noexcept
	{
		return (static_cast<std::uint64_t>(generation) << 32U) |
			(static_cast<std::uint64_t>(index) + 1U);
	}

	void RuntimeWorld::IncrementGeneration(std::uint32_t& generation) noexcept
	{
		++generation;
		if (generation == 0U)
		{
			generation = 1U;
		}
	}
}
