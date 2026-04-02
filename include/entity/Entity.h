//
// Created by komvu on 2024-12-19.
//

#ifndef WINTHER_ENGINE_ENTITY_H
#define WINTHER_ENGINE_ENTITY_H


#include <string>
#include <component/Component.h>
#include <map>

namespace Winther
{

	class Entity
	{
	public:

		Entity(size_t id, std::string tag);

		const std::string&& GetTag();

		size_t GetId();

		bool Exists();

		~Entity();

	private:

		const size_t m_Id;
		const std::string m_Tag;
		bool m_Exists;

	};

} // Winther

#endif //WINTHER_ENGINE_ENTITY_H
