//
// Created by komvu on 2026-08-18.
//

#ifndef MODWIN_ENGINE_MAPSERIALIZER_H
#define MODWIN_ENGINE_MAPSERIALIZER_H
#include <filesystem>
#include "TileMap.h"


namespace Modwin
{
	class MapSerializer
	{
		public:
			static bool Save(const TileMap& map, const std::filesystem::path& destination);
	};
}


#endif //MODWIN_ENGINE_MAPSERIALIZER_H
