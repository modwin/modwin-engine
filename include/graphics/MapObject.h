#ifndef MODWIN_ENGINE_MAPOBJECT_H
#define MODWIN_ENGINE_MAPOBJECT_H
#include <string>
#include <unordered_map>

#include "physics/Vector2D.h"

namespace Modwin
{
	struct MapObject
	{
		std::string name;
		std::string type;
		Vector2D position;
		Vector2D size;
		std::unordered_map<std::string, std::string> properties;
	};
}
#endif //MODWIN_ENGINE_MAPOBJECT_H
