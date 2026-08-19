#ifndef MODWIN_ENGINE_EDITORCAMERA_H
#define MODWIN_ENGINE_EDITORCAMERA_H

#include "graphics/TileCoordinate.h"

namespace Modwin
{
	struct EditorCamera
	{
		float x = 0.0F;
		float y = 0.0F;
		float zoom = 1.0F;
	};

	void ClampEditorCamera(
		EditorCamera& camera,
		const MapViewport& viewport,
		float mapPixelWidth,
		float mapPixelHeight) noexcept;
}

#endif // MODWIN_ENGINE_EDITORCAMERA_H
