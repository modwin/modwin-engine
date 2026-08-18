#ifndef MODWIN_ENGINE_EDITORSTATE_H
#define MODWIN_ENGINE_EDITORSTATE_H
#include <cstddef>
#include "graphics/TileLayer.h"

namespace Modwin
{
	enum class EditorMode
	{
		Edit,
		Play
	};

	enum class EditorTool
	{
		Paint,
		Erase
	};

	struct EditorState
	{
		EditorMode mode = EditorMode::Edit;
		EditorTool activeTool = EditorTool::Paint;

		std::size_t activeLayerIndex = 0;
		TileGid selectedTile = 0;

		int hoveredColumn = -1;
		int hoveredRow = -1;

		bool documentDirty = false;
	};
}


#endif //MODWIN_ENGINE_EDITORSTATE_H
