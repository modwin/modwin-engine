#ifndef MODWIN_ENGINE_EDITORSTATE_H
#define MODWIN_ENGINE_EDITORSTATE_H
#include <cstddef>

#include "graphics/TileLayer.h"

enum class EditorMode
{
	Edit, Play
};

enum class EditorTool
{
	Paint, Erase
};

class EditorState
{
	public:
		EditorState();
		virtual ~EditorState();

		std::size_t activeLayer = 0;
		Modwin::TileGid selectedTile = 0;

		int hoveredColumn = -1;
		int hoveredRow = -1;

		bool documentDirty = false;
};


#endif //MODWIN_ENGINE_EDITORSTATE_H
