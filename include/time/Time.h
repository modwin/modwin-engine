#ifndef MODWIN_ENGINE_TIME_H
#define MODWIN_ENGINE_TIME_H

#include <cstdint>

namespace Modwin
{
	class FrameClock
	{
	public:
		static constexpr float FixedDeltaSeconds = 1.0F / 60.0F;
		static constexpr float MaximumFrameDeltaSeconds = 0.25F;
		static constexpr int MaximumFixedStepsPerFrame = 8;

		void Reset() noexcept;
		[[nodiscard]] float Tick() noexcept;

	private:
		std::uint64_t m_PreviousTicks = 0;
	};
}

#endif // MODWIN_ENGINE_TIME_H
