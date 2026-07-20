//
// Created by komvu on 2024-12-27.
//
#pragma once
#ifndef WINTHER_ENGINE_VECTOR2D_H
#define WINTHER_ENGINE_VECTOR2D_H

namespace Winther
{

	class Vector2D
	{
	public:

		Vector2D(float x, float y): X(x), Y(y){};



	public:
		float X, Y;


		void Normalize();

		double Length();

		inline bool operator==(const Vector2D& other) const
		{
			return X == other.X && Y == other.Y;
		}

		inline bool operator!=(const Vector2D& other) const
		{
			return X != other.X && Y != other.Y;
		}

		inline Vector2D operator+(const Vector2D& other) const
		{
			return Vector2D(X + other.X, Y + other.Y);
		}

		inline Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}

		inline Vector2D operator*(const float f) const
		{
			return Vector2D(X*f, Y*f);
		}

		void Log();

		~Vector2D();

	};

} // Winther

#endif //WINTHER_ENGINE_VECTOR2D_H
