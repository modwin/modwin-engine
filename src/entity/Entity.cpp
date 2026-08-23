#include "entity/Entity.h"

#include "graphics/TextureManager.h"

#include <utility>

namespace Modwin
{
	Entity::Entity(std::string tag, const EntityId id, EntityDescriptor descriptor)
		: m_Transform(descriptor.position),
		  m_Descriptor(std::move(descriptor)),
		  m_Id(id),
		  m_Tag(std::move(tag))
	{
	}

	void Entity::FixedUpdate(float)
	{
	}

	void Entity::Update(float)
	{
	}

	void Entity::Draw()
	{
		if (!m_Active)
		{
			return;
		}

		TextureManager::GetInstance()->Draw(
			m_Descriptor.textureId,
			m_Transform.GetX(),
			m_Transform.GetY(),
			m_Descriptor.spriteSize.X,
			m_Descriptor.spriteSize.Y,
			m_Descriptor.flipMode);
	}
}
