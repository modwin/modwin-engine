#ifndef MODWIN_ENGINE_EDITORUI_H
#define MODWIN_ENGINE_EDITORUI_H
#include "EditorState.h"
#include "graphics/MapDocument.h"
#include "graphics/TextureManager.h"
#include "graphics/TileCoordinate.h"


namespace Modwin
{
	enum class CloseDecision
	{
		None,
		SaveAndClose,
		DiscardAndClose,
		Cancel
	};

	class EditorUI
	{
	public:
		[[nodiscard]] MapViewport Draw(
			EditorState& state,
			MapDocument& document,
			TextureManager& textures) const;

		[[nodiscard]] CloseDecision DrawCloseConfirmation(
			MapDocument& document, bool openPopup) const;
	};
}



#endif // MODWIN_ENGINE_EDITORUI_H
