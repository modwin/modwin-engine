//
// Created by komvu on 2024-12-28.
//

#ifndef WINTHER_ENGINE_CHARACTER_H
#define WINTHER_ENGINE_CHARACTER_H

#include <utility>
#include <entity/Entity.h>

namespace Winther
{

class Character : public Entity
	{
	public:
		Character(std::string tag, size_t id, Properties properties) : Entity(std::move(tag), id, properties)
		{
			std::cout << "Properties i Character konstruktorn = " << properties.h << std::endl;

		};
		virtual ~Character()= default;
	protected:
		std::string m_Name;
	private:


	};
}

#endif //WINTHER_ENGINE_CHARACTER_H
