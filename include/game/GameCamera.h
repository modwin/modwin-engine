#ifndef MODWIN_ENGINE_GAMECAMERA_H
#define MODWIN_ENGINE_GAMECAMERA_H

#include "graphics/TileCoordinate.h"
namespace Modwin
{
	struct GameCamera
	{
		float x = 0.0F;
		float y = 0.0F;
		float zoom = 1.0F;
	};

	void ClampEditorCamera(
		GameCamera& camera,
		const MapViewport& viewport,
		float mapPixelWidth,
		float mapPixelHeight) noexcept;

}
#endif //MODWIN_ENGINE_GAMECAMERA_H
