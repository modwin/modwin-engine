#include "time/Time.h"

#include <SDL3/SDL.h>

#include <algorithm>

namespace Modwin
{
	void FrameClock::Reset() noexcept
	{
		m_PreviousTicks = SDL_GetTicksNS();
	}

	float FrameClock::Tick() noexcept
	{
		const std::uint64_t currentTicks = SDL_GetTicksNS();
		if (m_PreviousTicks == 0)
		{
			m_PreviousTicks = currentTicks;
			return 0.0F;
		}

		const std::uint64_t elapsedTicks = currentTicks - m_PreviousTicks;
		m_PreviousTicks = currentTicks;
		constexpr double nanosecondsToSeconds = 1.0 / 1'000'000'000.0;
		const float elapsedSeconds =
			static_cast<float>(static_cast<double>(elapsedTicks) * nanosecondsToSeconds);
		return std::clamp(elapsedSeconds, 0.0F, MaximumFrameDeltaSeconds);
	}
}
