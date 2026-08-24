#ifndef MODWIN_ENGINE_INPUTHANDLER_H
#define MODWIN_ENGINE_INPUTHANDLER_H

#include <SDL3/SDL.h>

#include <cstdint>
#include <vector>

namespace Modwin
{
	class InputHandler
	{
	public:
		static InputHandler* GetInstance();

		void ProcessEvents();
		[[nodiscard]] bool ConsumeQuitRequest() noexcept;
		[[nodiscard]] bool IsKeyDown(SDL_Scancode key) const noexcept;
		[[nodiscard]] bool WasKeyPressed(SDL_Scancode key) const noexcept;
		[[nodiscard]] bool WasKeyReleased(SDL_Scancode key) const noexcept;

	private:
		InputHandler() = default;

		[[nodiscard]] bool IsValidKey(SDL_Scancode key) const noexcept;

		std::vector<std::uint8_t> m_CurrentKeys;
		std::vector<std::uint8_t> m_PreviousKeys;
		bool m_QuitRequested = false;
	};
}

#endif // MODWIN_ENGINE_INPUTHANDLER_H
