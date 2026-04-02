//
// Created by komvu on 2024-12-19.
//

#include "Entity.h"

#include <utility>

namespace Winther
{
	Entity::Entity(size_t id, std::string  tag): m_Tag(std::move(tag)), m_Id(id){}

	Entity::~Entity()
	{
		delete this;
	}

	const std::string&& Entity::GetTag()
	{
		return const_cast<std::string&&>(m_Tag);
	}

	size_t Entity::GetId()
	{
		return m_Id;
	}

	bool Entity::Exists()
	{
		return m_Exists;
	};


} // Winther