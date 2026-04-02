//
// Created by komvu on 2024-12-19.
//

#include "EntityManager.h"

namespace Winther
{
	std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
	{
		return std::shared_ptr<Entity>();
	}

	EntityManager::~EntityManager()
	{

	}

	EntityManager::EntityManager()
	{

	}

	/*
	 * Updates the state of all Entities before each
	 * frame has been rendered in order to ensure safe
	 * iteration of m_entities.
	 */

	void EntityManager::Update()
	{

	}



	/*
	 * Filters out Entities with a certain tag.
	 * When calling this function, remove the returned
	 * vector should be removed.
	 */
	EntityVector& EntityManager::GetEntitiesWithName(const std::string& tag)
	{
		return m_entityTagMap.at(tag);
	}

	EntityVector& EntityManager::GetEntities()
	{
		return m_entities;
	}

	const EntityVector& EntityManager::GetStaticEntities()
	{
		return m_staticEntities;

	}

	EntityVector& EntityManager::GetEntityWithComponent(const std::string& cTag)
	{
		return m_entityComponentMap.at(cTag);
	}
} // Winther