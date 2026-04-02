//
// Created by komvu on 2024-12-27.
//

#include "component/Vector2D.h"

namespace Winther
{
	Vector2D::Vector2D(double x, double y): m_X(x), m_Y(y){};

	Vector2D::~Vector2D()
	{
		m_X = 0;
		m_Y = 0;
	}

	double Vector2D::Length()
	{
		return 0;
	}

	void Vector2D::Normalize()
	{

	}

	bool Vector2D::operator==(const Vector2D& other) const
	{
		return m_X == other.m_X && m_Y == other.m_Y;
	}

	bool Vector2D::operator!=(const Vector2D& other) const
	{
		return m_X != other.m_X && m_Y != other.m_Y;
	}


} // Winther