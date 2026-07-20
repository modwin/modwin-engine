//
// Created by komvu on 2024-12-19.
//

#include "entity/Entity.h"

#include <utility>

namespace Modwin
{

	Entity::Entity(std::string  tag, const size_t id, const Properties& properties) : m_RigidBody(nullptr), m_Animation(nullptr),
	                                                                     m_Transform(new Transform(properties.x,
		                                                                     properties.y)),
	                                                                     m_Properties(properties), m_TextureID(properties.m_TextureID),
	                                                                     m_Id(id),
	                                                                     m_Tag(std::move(tag)),
	                                                                     m_Exists(true)
	{
	}

	Entity::Entity(const Entity& other) : m_Properties(other.m_Properties), m_TextureID(other.m_TextureID), m_Id(other.m_Id), m_Tag(other.m_Tag)
	{
		m_Animation = other.m_Animation;
		m_Exists = true;
		m_Transform = other.m_Transform;
		m_RigidBody = other.m_RigidBody;
	}

	Entity::~Entity()
	{
		m_Transform = nullptr;
		m_RigidBody = nullptr;
		m_Animation = nullptr;
		m_Exists = false;
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
		if(m_Animation != nullptr)
		{
			m_Animation->Update();
			m_RigidBody->Update(dt);
		}
	}

	const std::string&& Entity::GetTag()
	{
		return const_cast<std::string&&>(m_Tag);
	}

	size_t Entity::GetId() const
	{
		return m_Id;
	}

	bool Entity::Exists() const
	{
		return m_Exists;
	}

	void Entity::Clean()
	{
		m_Animation = nullptr;
		m_Transform = nullptr;
		m_RigidBody = nullptr;
	}


} // Modwin