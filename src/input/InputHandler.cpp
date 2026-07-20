//
// Created by komvu on 2025-01-03.
//

#include "input/InputHandler.h"
#include <core/Engine.h>

namespace Winther
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

	bool InputHandler::EventListener()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch(event.type)
			{
			case SDL_EVENT_QUIT:
				Engine::GetInstance()->Quit();
				return false;
			case SDL_EVENT_KEY_DOWN:
//				KeyDown(event);
				GetKeyDown(event.key.scancode);
				break;
			case SDL_EVENT_KEY_UP:
//				KeyUp(event);
				break;
			}
		}
		return true;
	}

	bool InputHandler::GetKeyDown(SDL_Scancode key)
	{
		return m_KeyStates[key] == 1;
		std::cout << "En knapp har tryckts ned! -> "<< SDL_GetScancodeName(key) << std::endl;
		;
//		return m_KeyStates[key] == 1;
	}

//	void InputHandler::KeyUp(SDL_Event& e)
//	{
//		SDL_Log("Wow, you just RELEASED the %s key!", SDL_GetKeyName(e.key.key));
//		m_KeyStates = SDL_GetKeyboardState(nullptr);
//	}
//
//	void InputHandler::KeyDown(SDL_Event& e)
//	{
//		SDL_Log("Wow, you just PRESSED the %s key!", SDL_GetKeyName(e.key.key));
//		m_KeyStates = SDL_GetKeyboardState(nullptr);
//	}
}