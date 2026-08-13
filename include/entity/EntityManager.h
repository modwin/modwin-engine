#ifndef MODWIN_ENGINE_ENTITYMANAGER_H
#define MODWIN_ENGINE_ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <entity/Entity.h>
#include <map>



namespace Modwin
{

	typedef std::vector<std::shared_ptr<Entity>> EntityVector;
	typedef std::map<std::string, EntityVector> EntityTagMap;

	class EntityManager
	{

	public:
		~EntityManager();

		static EntityManager* GetInstance();

		std::shared_ptr<Entity> AddEntity(const std::string& tag, Properties properties);

		EntityVector& GetEntities();

		EntityVector& GetEntitiesWithName(const std::string& tag);

		[[nodiscard]] size_t GetEntityCount() const noexcept { return m_entityCount; }

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

} // Modwin

#endif //MODWIN_ENGINE_ENTITYMANAGER_H

