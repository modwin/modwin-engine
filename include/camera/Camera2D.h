#ifndef MODWIN_ENGINE_CAMERA_H
#define MODWIN_ENGINE_CAMERA_H
#include "graphics/TileCoordinate.h"
#include "physics/Vector2D.h"
#include <algorithm>

namespace Modwin
{
	class Camera2D
	{
		public:
			Vector2D position{0.0F, 0.0F};
			float zoom = 1.0F;

			[[nodiscard]] float GetVisibleWidth(const MapViewport& vp) const noexcept
			{
				return vp.width / zoom;
			}

			[[nodiscard]] float GetVisibleHeight(const MapViewport& vp) const noexcept
			{
				return vp.height / zoom;
			}

			void CenterOn(const Vector2D centerWorldPos, const MapViewport& vp) noexcept
			{
				position.X = centerWorldPos.X - (GetVisibleWidth(vp) * 0.5F);
				position.Y = centerWorldPos.Y - (GetVisibleHeight(vp) * 0.5F);
			}

			void ClampToMap(const MapViewport& vp, float mapPixelWidth, float mapPixelHeight) noexcept
			{
				zoom = std::clamp(zoom, 0.25F, 4.0F);
				const float visW = GetVisibleWidth(vp);
				const float visH = GetVisibleHeight(vp);

				if (mapPixelWidth <= visW)
					position.X = -(visW - mapPixelWidth) * 0.5F;
				else
					position.X = std::clamp(position.X, 0.0F, mapPixelWidth - visW);

				if (mapPixelHeight <= visH)
					position.Y = -(visH - mapPixelHeight) * 0.5F;
				else
					position.Y = std::clamp(position.Y, 0.0F, mapPixelHeight - visH);
			}
	};
}

#endif //MODWIN_ENGINE_CAMERA_H
