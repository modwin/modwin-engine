#ifndef MODWIN_ENGINE_CHARACTER_H
#define MODWIN_ENGINE_CHARACTER_H

#include <utility>
#include <entity/Entity.h>

namespace Modwin
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

#endif //MODWIN_ENGINE_CHARACTER_H
