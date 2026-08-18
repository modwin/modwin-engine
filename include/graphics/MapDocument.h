#ifndef MODWIN_ENGINE_MAPDOCUMENT_H
#define MODWIN_ENGINE_MAPDOCUMENT_H
#include <filesystem>
#include "TileMap.h"

namespace Modwin
{
	struct MapDocument
	{
		std::filesystem::path sourcePath;
		TileMap map;
		bool dirty = false;
	};
}


#endif //MODWIN_ENGINE_MAPDOCUMENT_H
