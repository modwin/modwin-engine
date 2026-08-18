#ifndef MODWIN_ENGINE_MAPSERIALIZER_H
#define MODWIN_ENGINE_MAPSERIALIZER_H

#include "graphics/TileMap.h"

#include <filesystem>


namespace Modwin
{
	class MapSerializer
	{
		public:
			static bool Save(const TileMap& map, const std::filesystem::path& destination);
	};
}


#endif // MODWIN_ENGINE_MAPSERIALIZER_H
