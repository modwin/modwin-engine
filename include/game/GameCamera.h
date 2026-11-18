#ifndef MODWIN_ENGINE_GAMECAMERA_H
#define MODWIN_ENGINE_GAMECAMERA_H
#include "camera/Camera2D.h"
#include "graphics/TileCoordinate.h"
#include "modwin/abi/ModwinGameApi.h"

namespace Modwin
{
    enum class CameraFollowMode
    {
        Manual,
        FollowEntity
    };

    struct CameraDeadzone
    {
        float widthPercent = 0.40F;
        float heightPercent = 0.30F;
    };

    class GameCamera
    {
        public:
            void SetFollowTarget(ModwinEntityHandle entity) noexcept
            {
                m_TargetEntity = entity;
                m_Mode = CameraFollowMode::FollowEntity;
            }

            void SetManualPosition(const Vector2D pos) noexcept
            {
                m_DesiredPosition = pos;
                m_Mode = CameraFollowMode::Manual;
            }

            void SetSmoothingSpeed(float speed) noexcept { m_SmoothSpeed = speed; }
            void SetTargetZoom(float targetZoom) noexcept { m_TargetZoom = targetZoom; }

            [[nodiscard]] Camera2D& GetCamera() noexcept { return m_Camera; }
            [[nodiscard]] const Camera2D& GetCamera() const noexcept { return m_Camera; }

            void Update(
                const MapViewport& viewport,
                float mapPixelWidth,
                float mapPixelHeight,
                float deltaSeconds,
                const class RuntimeWorld& runtimeWorld);

        private:
            Camera2D m_Camera;
            CameraFollowMode m_Mode = CameraFollowMode::Manual;
            ModwinEntityHandle m_TargetEntity = MODWIN_INVALID_ENTITY_HANDLE;
            CameraDeadzone m_Deadzone;

            Vector2D m_DesiredPosition{0.0F, 0.0F};
            float m_TargetZoom = 1.0F;
            float m_SmoothSpeed = 5.0F;
    };
}
#endif //MODWIN_ENGINE_GAMECAMERA_H
