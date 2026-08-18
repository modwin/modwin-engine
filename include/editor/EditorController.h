#ifndef MODWIN_ENGINE_EDITORCONTROLLER_H
#define MODWIN_ENGINE_EDITORCONTROLLER_H

#include "editor/EditorState.h"
#include "graphics/MapDocument.h"
#include "graphics/TileCoordinate.h"

namespace Modwin
{
	class EditorController
	{
	public:
		void Update(
			EditorState& state,
			MapDocument& document,
			const MapViewport& viewport) const;
	};
}

#endif // MODWIN_ENGINE_EDITORCONTROLLER_H
