#ifndef MODWIN_ENGINE_ENTITY_H
#define MODWIN_ENGINE_ENTITY_H

#include "physics/Transform.h"

#include <SDL3/SDL.h>

#include <cstddef>
#include <string>
#include <utility>

namespace Modwin
{
	using EntityId = std::size_t;

	struct EntityDescriptor
	{
		Vector2D position{};
		Vector2D spriteSize{};
		SDL_FlipMode flipMode = SDL_FLIP_NONE;
		std::string textureId;
		std::string textureDirectory;
	};

	class Entity
	{
	public:
		Entity(std::string tag, EntityId id, EntityDescriptor descriptor);
		virtual ~Entity() = default;

		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		[[nodiscard]] const std::string& GetTag() const noexcept { return m_Tag; }
		[[nodiscard]] EntityId GetId() const noexcept { return m_Id; }
		[[nodiscard]] bool IsActive() const noexcept { return m_Active; }
		void SetActive(bool active) noexcept { m_Active = active; }

		[[nodiscard]] Transform& GetTransform() noexcept { return m_Transform; }
		[[nodiscard]] const Transform& GetTransform() const noexcept { return m_Transform; }

		virtual void FixedUpdate(float deltaSeconds);
		virtual void Update(float deltaSeconds);
		virtual void Draw();

	protected:
		[[nodiscard]] const EntityDescriptor& GetDescriptor() const noexcept
		{
			return m_Descriptor;
		}

	private:
		Transform m_Transform;
		EntityDescriptor m_Descriptor;
		EntityId m_Id;
		std::string m_Tag;
		bool m_Active = true;
	};
}

#endif // MODWIN_ENGINE_ENTITY_H
