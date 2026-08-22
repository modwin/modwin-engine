#ifndef MODWIN_ENGINE_TIME_H
#define MODWIN_ENGINE_TIME_H

namespace Modwin
{
	constexpr int FPS_LIMIT = 60;
	constexpr float DELTA_TIME_MAX = 1.5f;

	class Time
	{
		public:
			static Time* GetInstance();

			void CalculateTickRate();

			[[nodiscard]] float GetDeltaTime() const { return m_DeltaTime; }
			[[nodiscard]] float GetLastTIme() const { return m_LastTime; }

		private:
			Time()
			{
			};

			~Time();

			float m_DeltaTime;
			float m_LastTime;
			static Time* s_INSTANCE;
	};
}


#endif //MODWIN_ENGINE_TIME_H
