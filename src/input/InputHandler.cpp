#include "input/InputHandler.h"

#include <imgui_impl_sdl3.h>

#include <algorithm>

namespace Modwin
{
	InputHandler* InputHandler::GetInstance()
	{
		static InputHandler instance;
		return &instance;
	}

	void InputHandler::ProcessEvents()
	{
		m_PreviousKeys = m_CurrentKeys;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
			{
				m_QuitRequested = true;
			}
		}

		int keyCount = 0;
		const bool* keys = SDL_GetKeyboardState(&keyCount);
		m_CurrentKeys.resize(static_cast<std::size_t>(keyCount));
		for (int index = 0; index < keyCount; ++index)
		{
			m_CurrentKeys[static_cast<std::size_t>(index)] = keys[index] ? 1U : 0U;
		}

		if (m_PreviousKeys.size() != m_CurrentKeys.size())
		{
			m_PreviousKeys.resize(m_CurrentKeys.size(), 0U);
		}
	}

	bool InputHandler::ConsumeQuitRequest() noexcept
	{
		const bool requested = m_QuitRequested;
		m_QuitRequested = false;
		return requested;
	}

	bool InputHandler::IsKeyDown(const SDL_Scancode key) const noexcept
	{
		return IsValidKey(key) && m_CurrentKeys[static_cast<std::size_t>(key)] != 0U;
	}

	bool InputHandler::WasKeyPressed(const SDL_Scancode key) const noexcept
	{
		if (!IsValidKey(key))
		{
			return false;
		}

		const auto index = static_cast<std::size_t>(key);
		return m_CurrentKeys[index] != 0U && m_PreviousKeys[index] == 0U;
	}

	bool InputHandler::WasKeyReleased(const SDL_Scancode key) const noexcept
	{
		if (!IsValidKey(key))
		{
			return false;
		}

		const auto index = static_cast<std::size_t>(key);
		return m_CurrentKeys[index] == 0U && m_PreviousKeys[index] != 0U;
	}

	bool InputHandler::IsValidKey(const SDL_Scancode key) const noexcept
	{
		const int index = static_cast<int>(key);
		return index >= 0 && static_cast<std::size_t>(index) < m_CurrentKeys.size();
	}
}
