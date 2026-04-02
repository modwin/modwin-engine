//
// Created by komvu on 2024-12-19.
//

#ifndef WINTHER_ENGINE_ENTITYMANAGER_H
#define WINTHER_ENGINE_ENTITYMANAGER_H
#include <Entity.h>
#include <vector>
#include <memory>
#include <map>


namespace Winther
{

	typedef std::map<std::string, Component> ComponentMap;
	typedef std::vector<std::shared_ptr<Entity>> EntityVector;
	typedef std::map<std::string, EntityVector> EntityTagMap;
	typedef std::map<std::string, EntityVector> EntityComponentMap;

	class EntityManager
	{
	private:
		ComponentMap m_components;
		EntityVector m_staticEntities;
		EntityVector m_entities;
		EntityTagMap m_entityTagMap;
		EntityComponentMap m_entityComponentMap;
		size_t		 size;

	public:

		EntityManager();

		std::shared_ptr<Entity> addEntity(const std::string& tag);

		EntityVector& GetEntities();

		EntityVector& GetEntityWithComponent(const std::string& cTag);

		EntityVector& GetEntitiesWithName(const std::string& tag);

		const EntityVector& GetStaticEntities();

		~EntityManager();


		void Update();
	};

} // Winther

#endif //WINTHER_ENGINE_ENTITYMANAGER_H
