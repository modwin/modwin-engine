#include "game/GameCamera.h"
#include "runtime/RuntimeWorld.h"
#include "physics/Vector2D.h"
#include <cmath>

namespace Modwin
{
	void GameCamera::Update(
		const MapViewport& viewport,
		const float mapPixelWidth,
		const float mapPixelHeight,
		const float deltaSeconds,
		const RuntimeWorld& runtimeWorld)
	{
		if (m_Mode == CameraFollowMode::FollowEntity)
		{
			Vector2D targetPos{};
			if (runtimeWorld.GetPosition(m_TargetEntity, targetPos))
			{
				const float visW = m_Camera.GetVisibleWidth(viewport);
				const float visH = m_Camera.GetVisibleHeight(viewport);

				const float deadzoneW = visW * m_Deadzone.widthPercent;
				const float deadzoneH = visH * m_Deadzone.heightPercent;

				const float deadzoneLeft = m_DesiredPosition.X + (visW - deadzoneW) * 0.5F;
				const float deadzoneRight = deadzoneLeft + deadzoneW;
				const float deadzoneTop = m_DesiredPosition.Y + (visH - deadzoneH) * 0.5F;
				const float deadzoneBottom = deadzoneTop + deadzoneH;

				if (targetPos.X < deadzoneLeft)
					m_DesiredPosition.X -= (deadzoneLeft - targetPos.X);
				else if (targetPos.X > deadzoneRight)
					m_DesiredPosition.X += (targetPos.X - deadzoneRight);

				if (targetPos.Y < deadzoneTop)
					m_DesiredPosition.Y -= (deadzoneTop - targetPos.Y);
				else if (targetPos.Y > deadzoneBottom)
					m_DesiredPosition.Y += (targetPos.Y - deadzoneBottom);
			}
			else
			{
				m_Mode = CameraFollowMode::Manual;
			}
		}
		const float t = 1.0F - std::exp(-m_SmoothSpeed * deltaSeconds);
		m_Camera.position.X = Lerp(m_Camera.position.X, m_DesiredPosition.X, t);
		m_Camera.position.Y = Lerp(m_Camera.position.Y, m_DesiredPosition.Y, t);

		m_Camera.zoom = Lerp(m_Camera.zoom, m_TargetZoom, t);

		m_Camera.ClampToMap(viewport, mapPixelWidth, mapPixelHeight);
	}
}
