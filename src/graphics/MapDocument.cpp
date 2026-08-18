#include "graphics/MapDocument.h"

#include <utility>

namespace Modwin
{
	MapDocument::MapDocument(std::filesystem::path sourcePath, TileMap map)
		: sourcePath(std::move(sourcePath)), map(std::move(map))
	{
	}

	void MapDocument::MarkDirty() noexcept
	{
		dirty = true;
	}

	void MapDocument::MarkSaved() noexcept
	{
		dirty = false;
	}

	bool MapDocument::IsDirty() const noexcept
	{
		return dirty;
	}
}
