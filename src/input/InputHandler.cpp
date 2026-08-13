#include "input/InputHandler.h"
#include <imgui_impl_sdl3.h>

namespace Modwin
{
	InputHandler* InputHandler::s_INSTANCE = nullptr;

	InputHandler* InputHandler::GetInstance()
	{
		if(s_INSTANCE == nullptr)
		{
			s_INSTANCE = new InputHandler();
		}
		return s_INSTANCE;
	}

	bool InputHandler::ProcessEvents()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
			{
				return false;
			}
		}
		return true;
	}

	bool InputHandler::IsKeyDown(SDL_Scancode key) const
	{
		return m_KeyStates[key];
	}
}
