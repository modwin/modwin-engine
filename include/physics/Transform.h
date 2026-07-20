//
// Created by komvu on 2024-12-27.
//

#ifndef WINTHER_ENGINE_TRANSFORM_H
#define WINTHER_ENGINE_TRANSFORM_H

#include <physics/Vector2D.h>
#include <iostream>

namespace Modwin
{

	class Transform
	{
	private:
		float m_X, m_Y;

		inline void Translate(const Vector2D& vec)
		{
			m_X += vec.X;
			m_Y += vec.Y;
		}

	public:
		Transform(float x = 0, float y = 0) : m_X(x), m_Y(y){};

		inline float GetX()
		{
			return m_X;
		};

		inline float GetY()
		{
			return m_Y;
		}

		inline void TranslateX(float x)
		{
			m_X += x;
		}

		inline void TranlateY(float y)
		{
			m_Y += y;
		}

		inline void Log()
		{
			std::cout << "Transform = {x = " << m_X << ", y = " << m_Y << "}" << std::endl;
		}


	};

}


#endif //WINTHER_ENGINE_TRANSFORM_H
