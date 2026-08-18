#include "editor/EditorUI.h"

#include <iostream>

namespace Modwin
{
	void EditorUI::Draw(EditorState& state, MapDocument& document, TextureManager& textures)
	{
		if (state.mode == EditorMode::Play || document.dirty)
		{
			return;
		}
		std::cout << std::endl << std::endl << std::endl;
	}
}
