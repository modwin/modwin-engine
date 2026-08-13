#include "time/Time.h"
#include <SDL3/SDL.h>

namespace Modwin
{
	Time* Time::s_INSTANCE = nullptr;

	Time* Time::GetInstance()
	{
		if(s_INSTANCE == nullptr)
		{
			s_INSTANCE = new Time();
		}
		return s_INSTANCE;
	}

	void Time::CalculateTickRate()
	{
		m_DeltaTime = (SDL_GetTicks() - m_LastTime) * FPS_LIMIT / 1000.0f;

		if(m_DeltaTime > DELTA_TIME_MAX)
		{
			m_DeltaTime = DELTA_TIME_MAX;
		}

		m_LastTime = SDL_GetTicks();

	}


}
