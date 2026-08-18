#ifndef MODWIN_ENGINE_EDITORUI_H
#define MODWIN_ENGINE_EDITORUI_H
#include "EditorState.h"
#include "graphics/MapDocument.h"
#include "graphics/TextureManager.h"


namespace Modwin
{
	class EditorUI
	{
		public:
			void Draw(EditorState & state, MapDocument & document, TextureManager & textures);
	};
}



#endif //MODWIN_ENGINE_EDITORUI_H
