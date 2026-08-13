#ifndef MODWIN_ENGINE_INPUTHANDLER_H
#define MODWIN_ENGINE_INPUTHANDLER_H

#include <SDL3/SDL.h>

namespace Modwin
{
	class InputHandler
	{
	public:

		static InputHandler* GetInstance();

		bool ProcessEvents();
		bool IsKeyDown(SDL_Scancode sdlKey) const;

	private:
		InputHandler(): m_KeyStates(SDL_GetKeyboardState(nullptr)){}
		~InputHandler() = default;

		const bool* m_KeyStates;
		static InputHandler* s_INSTANCE;


	};
}//Modwin


#endif //MODWIN_ENGINE_INPUTHANDLER_H
