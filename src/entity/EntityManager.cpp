//
// Created by komvu on 2024-12-19.
//

#include "entity/EntityManager.h"
#include "character/Player.h"

namespace Modwin
{

	EntityManager* EntityManager::s_INSTANCE = nullptr;

	EntityManager* EntityManager::GetInstance()
	{
		if (s_INSTANCE == nullptr)
		{
			s_INSTANCE = new EntityManager();
		}
		return s_INSTANCE;
	}

	std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag, Properties properties)
	{
		if(tag == "player")
		{
			SDL_Log("Properties H = %d", properties.h);
			auto p = std::make_shared<Player>(tag, ++m_entityCount, properties);
			SDL_Log("Player height = %d", p->m_Properties.h);
			m_Entities.push_back(static_cast<std::shared_ptr<Entity>>(p));
			return p;
		}
		else {
			auto e = std::make_shared<Entity>(tag, ++m_entityCount, properties);
			m_Entities.push_back(e);
			return e;
		}
		return nullptr;
	}

	/*
	 * Updates the state of all Entities before each
	 * frame has been rendered in order to ensure safe
	 * iteration of m_Entities.
	 */

	void EntityManager::Update(float deltaTime)
	{
		for(const auto& e : m_Entities)
		{
			e->Update(deltaTime);
		}
//		CalculateGravity();
//		CalculatePosition();
	}

	void EntityManager::Draw()
	{
		for(const auto& e : m_Entities)
		{
			e->Draw();
		}

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
		return m_Entities;
	}

	const EntityVector& EntityManager::GetStaticEntities()
	{
		return m_staticEntities;

	}

	void EntityManager::Clean()
	{
		for(auto e : m_Entities)
		{
			e->Clean();
			e = nullptr;
		}

	}

	EntityManager::~EntityManager()
	{
		m_Entities.clear();
		m_staticEntities.clear();
		m_entityTagMap.clear();
	}


} // Modwin