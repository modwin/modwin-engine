#include "editor/EditorCamera.h"

#include <algorithm>

namespace Modwin
{
	void ClampEditorCamera(
		EditorCamera& camera,
		const MapViewport& viewport,
		const float mapPixelWidth,
		const float mapPixelHeight) noexcept
	{
		camera.zoom = std::clamp(camera.zoom, 0.25F, 4.0F);
		if (viewport.width <= 0.0F || viewport.height <= 0.0F ||
			mapPixelWidth <= 0.0F || mapPixelHeight <= 0.0F)
		{
			camera.x = 0.0F;
			camera.y = 0.0F;
			return;
		}

		const float visibleWorldWidth = viewport.width / camera.zoom;
		const float visibleWorldHeight = viewport.height / camera.zoom;
		const float maximumX = std::max(0.0F, mapPixelWidth - visibleWorldWidth);
		const float maximumY = std::max(0.0F, mapPixelHeight - visibleWorldHeight);
		camera.x = std::clamp(camera.x, 0.0F, maximumX);
		camera.y = std::clamp(camera.y, 0.0F, maximumY);
	}
}
