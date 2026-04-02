//
// Created by komvu on 2024-12-27.
//

#ifndef WINTHER_ENGINE_VECTOR2D_H
#define WINTHER_ENGINE_VECTOR2D_H

namespace Winther
{

	class Vector2D
	{
	private:
		double m_X;
		double m_Y;

		bool operator==(const Vector2D& other) const;

		bool operator!=(const Vector2D& other) const;

	public:
		Vector2D(double x, double y);

		void Normalize();

		double Length();


		~Vector2D();

	};

} // Winther

#endif //WINTHER_ENGINE_VECTOR2D_H
