//
// Created by komvu on 2025-01-03.
//

#ifndef WINTHER_ENGINE_CONFIG_H
#define WINTHER_ENGINE_CONFIG_H

#include <map>
#include <string>
#include <SDL3/SDL.h>


namespace Winther
{
	class Config
	{
	public:

		void SetMoveUpKey(SDL_Scancode sdlKey);
		void SetMoveRightKey(SDL_Scancode sdlKey);
		void SetMoveLeftKey(SDL_Scancode sdlKey);
		void SetAttackKey(SDL_Scancode sdlKey);
		void SetMoveDownKey(SDL_Scancode sdlKey);
		SDL_Scancode GetMoveUpKey();
		SDL_Scancode GetMoveRightKey();
		SDL_Scancode GetMoveLeftKey();
		SDL_Scancode GetAttackKey();
		SDL_Scancode GetMoveDownKey();


		std::map<std::string, SDL_Scancode> m_KeyMap;
		static  std::map<std::string, SDL_Scancode> s_DEFAULT_KEY_MAP;

		Config();

		~Config();
	private:

	};
}


#endif //WINTHER_ENGINE_CONFIG_H
