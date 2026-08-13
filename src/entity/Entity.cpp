#include "entity/Entity.h"
#include <utility>

namespace Modwin
{

	Entity::Entity(std::string tag, const size_t id, const Properties& properties) : m_RigidBody(nullptr), m_Animation(nullptr),
	                                                                     m_Transform(std::make_unique<Transform>(properties.x,
		                                                                     properties.y)),
	                                                                     m_Properties(properties), m_TextureID(properties.m_TextureID),
	                                                                     m_Id(id),
	                                                                     m_Tag(std::move(tag)),
	                                                                     m_Exists(true)
	{
	}

	void Entity::Draw()
	{
		if(m_Animation != nullptr)
		{
			m_Animation->Draw(m_Properties.x, m_Properties.y, m_Properties.w, m_Properties.h);
		}
		else
		{
			TextureManager::GetInstance()->Draw(m_TextureID, m_Properties.x, m_Properties.y, m_Properties.w, m_Properties.h);
		}
	}
	void Entity::Update(float dt)
	{
		if(m_Animation)
		{
			m_Animation->Update();
		}

		if(m_RigidBody)
		{
			m_RigidBody->Update(dt);
		}
	}

	const std::string& Entity::GetTag() const noexcept
	{
		return m_Tag;
	}

	size_t Entity::GetId() const noexcept
	{
		return m_Id;
	}

	bool Entity::Exists() const noexcept
	{
		return m_Exists;
	}

} // Modwin
