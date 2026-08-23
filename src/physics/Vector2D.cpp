#include "physics/Vector2D.h"

#include <cmath>

namespace Modwin
{
	float Vector2D::LengthSquared() const noexcept
	{
		return X * X + Y * Y;
	}

	float Vector2D::Length() const noexcept
	{
		return std::sqrt(LengthSquared());
	}

	Vector2D Vector2D::Normalized() const noexcept
	{
		const float length = Length();
		return length > 0.0F ? *this / length : Vector2D{};
	}

	void Vector2D::Normalize() noexcept
	{
		*this = Normalized();
	}

	bool Vector2D::NearlyEquals(const Vector2D& other, const float epsilon) const noexcept
	{
		return std::abs(X - other.X) <= epsilon && std::abs(Y - other.Y) <= epsilon;
	}
}
