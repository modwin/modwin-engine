#ifndef MODWIN_ENGINE_EDITORCAMERA_H
#define MODWIN_ENGINE_EDITORCAMERA_H
#include "camera/Camera2D.h"
#include "graphics/TileCoordinate.h"

namespace Modwin
{
	void ClampEditorCamera(
		Camera2D& camera,
		const MapViewport& viewport,
		float mapPixelWidth,
		float mapPixelHeight) noexcept;
}

#endif // MODWIN_ENGINE_EDITORCAMERA_H
