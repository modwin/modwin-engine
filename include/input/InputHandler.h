//
// Created by komvu on 2025-01-03.
//

#ifndef WINTHER_ENGINE_INPUTHANDLER_H
#define WINTHER_ENGINE_INPUTHANDLER_H

#include <SDL3/SDL.h>
#include <core/Engine.h>

namespace Winther
{
	class InputHandler
	{
	public:

		static InputHandler* GetInstance();

		bool EventListener();
		bool GetKeyDown(SDL_Scancode sdlKey);

	private:
		InputHandler(): m_KeyStates(SDL_GetKeyboardState(nullptr)){};
		~InputHandler() = default;

		const bool* m_KeyStates;
		static InputHandler* s_INSTANCE;
		const int* m_NumKeys[];
		void KeyUp(SDL_Event& e);
		void KeyDown(SDL_Event& e);


	};
}//Winther


#endif //WINTHER_ENGINE_INPUTHANDLER_H
