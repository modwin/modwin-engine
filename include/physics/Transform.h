#ifndef MODWIN_ENGINE_TRANSFORM_H
#define MODWIN_ENGINE_TRANSFORM_H

#include "physics/Vector2D.h"

namespace Modwin
{
	class Transform
	{
	public:
		constexpr Transform() noexcept = default;
		constexpr Transform(float x, float y) noexcept : m_Position(x, y) {}
		explicit constexpr Transform(Vector2D position) noexcept : m_Position(position) {}

		[[nodiscard]] constexpr const Vector2D& GetPosition() const noexcept
		{
			return m_Position;
		}

		[[nodiscard]] constexpr float GetX() const noexcept { return m_Position.X; }
		[[nodiscard]] constexpr float GetY() const noexcept { return m_Position.Y; }

		constexpr void SetPosition(const Vector2D position) noexcept
		{
			m_Position = position;
		}

		constexpr void Translate(const Vector2D offset) noexcept
		{
			m_Position += offset;
		}

		constexpr void TranslateX(const float x) noexcept { m_Position.X += x; }
		constexpr void TranslateY(const float y) noexcept { m_Position.Y += y; }

	private:
		Vector2D m_Position{};
	};
}

#endif // MODWIN_ENGINE_TRANSFORM_H
