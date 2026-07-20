//
// Created by komvu on 2025-01-03.
//

#ifndef WINTHER_ENGINE_TIME_H
#define WINTHER_ENGINE_TIME_H

namespace Modwin
{
	const int FPS_LIMIT = 60;
	const float DELTA_TIME_MAX = 1.5f;

	class Time
	{
	public:
		static Time* GetInstance();

		void CalculateTickRate();
		inline float GetDeltaTime() const{return m_DeltaTime;}
		inline float GetLastTIme() const{return m_LastTime;}


	private:
		Time(){};
		~Time();
		float m_DeltaTime;
		float m_LastTime;
		static Time* s_INSTANCE;
	};
}


#endif //WINTHER_ENGINE_TIME_H
