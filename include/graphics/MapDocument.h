#ifndef MODWIN_ENGINE_MAPDOCUMENT_H
#define MODWIN_ENGINE_MAPDOCUMENT_H
#include "graphics/TileMap.h"

#include <filesystem>
#include <utility>

namespace Modwin
{
	struct MapDocument
	{
		MapDocument(std::filesystem::path sourcePath, TileMap map);

		void MarkDirty() noexcept;
		void MarkSaved() noexcept;
		[[nodiscard]] bool IsDirty() const noexcept;

		std::filesystem::path sourcePath;
		TileMap map;
		bool dirty = false;
	};
}


#endif // MODWIN_ENGINE_MAPDOCUMENT_H
