#ifndef MODWIN_ENGINE_VECTOR2D_H
#define MODWIN_ENGINE_VECTOR2D_H

namespace Modwin
{
	struct Vector2D
	{
		float X = 0.0F;
		float Y = 0.0F;

		constexpr Vector2D() noexcept = default;
		constexpr Vector2D(float x, float y) noexcept : X(x), Y(y) {}

		[[nodiscard]] float LengthSquared() const noexcept;
		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] Vector2D Normalized() const noexcept;
		void Normalize() noexcept;

		[[nodiscard]] bool NearlyEquals(
			const Vector2D& other, float epsilon = 0.0001F) const noexcept;

		[[nodiscard]] constexpr bool operator==(const Vector2D& other) const noexcept
		{
			return X == other.X && Y == other.Y;
		}

		[[nodiscard]] constexpr bool operator!=(const Vector2D& other) const noexcept
		{
			return !(*this == other);
		}

		[[nodiscard]] constexpr Vector2D operator+(const Vector2D& other) const noexcept
		{
			return {X + other.X, Y + other.Y};
		}

		[[nodiscard]] constexpr Vector2D operator-(const Vector2D& other) const noexcept
		{
			return {X - other.X, Y - other.Y};
		}

		[[nodiscard]] constexpr Vector2D operator*(float scalar) const noexcept
		{
			return {X * scalar, Y * scalar};
		}

		[[nodiscard]] constexpr Vector2D operator/(float scalar) const noexcept
		{
			return {X / scalar, Y / scalar};
		}

		constexpr Vector2D& operator+=(const Vector2D& other) noexcept
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		constexpr Vector2D& operator-=(const Vector2D& other) noexcept
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		constexpr Vector2D& operator*=(float scalar) noexcept
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}
	};

	[[nodiscard]] constexpr Vector2D operator*(float scalar, const Vector2D& vector) noexcept
	{
		return vector * scalar;
	}

	[[nodiscard]] inline constexpr float Lerp(const float a, const float b, const float t) noexcept
	{
		return a + (b - a) * t;
	}

	[[nodiscard]] inline Vector2D Lerp(const Vector2D a, const Vector2D b, const float t) noexcept
	{
		return {Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t)};
	}
}

#endif // MODWIN_ENGINE_VECTOR2D_H
