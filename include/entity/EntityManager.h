//
// Created by komvu on 2024-12-19.
//

#ifndef WINTHER_ENGINE_ENTITYMANAGER_H
#define WINTHER_ENGINE_ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <entity/Entity.h>
#include <map>
#include <SDL3/SDL.h>
#include <character/Character.h>
#include <character/Player.h>



namespace Winther
{

	typedef std::vector<std::shared_ptr<Entity>> EntityVector;
	typedef std::map<std::string, EntityVector> EntityTagMap;

	class EntityManager
	{

	public:
		~EntityManager();

		static EntityManager* GetInstance();

		std::shared_ptr<Entity> addEntity(const std::string& tag, Properties properties);

		EntityVector& GetEntities();

		EntityVector& GetEntitiesWithName(const std::string& tag);

		inline const size_t GetEntityCount(){ return m_entityCount;}

		const EntityVector& GetStaticEntities();

		void Update(float deltaTime);

		void Draw();

		void Clean();


	private:
		EntityManager(): m_entityCount(0){};
		static EntityManager* s_INSTANCE;
		EntityVector m_staticEntities;
		EntityVector m_Entities;
		EntityTagMap m_entityTagMap;
		size_t m_entityCount;

	};

} // Winther

#endif //WINTHER_ENGINE_ENTITYMANAGER_H

