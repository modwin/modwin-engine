//
// Created by komvu on 2025-01-03.
//

#ifndef WINTHER_ENGINE_CONFIG_H
#define WINTHER_ENGINE_CONFIG_H

#include <map>
#include <string>
#include <SDL3/SDL.h>


namespace Modwin
{
	class Config
	{
	public:

		void SetMoveUpKey(SDL_Scancode sdlKey);
		void SetMoveRightKey(SDL_Scancode sdlKey);
		void SetMoveLeftKey(SDL_Scancode sdlKey);
		void SetAttackKey(SDL_Scancode sdlKey);
		void SetMoveDownKey(SDL_Scancode sdlKey);
		SDL_Scancode GetMoveUpKey() const;
		SDL_Scancode GetMoveRightKey() const;
		SDL_Scancode GetMoveLeftKey() const;
		SDL_Scancode GetAttackKey() const;
		SDL_Scancode GetMoveDownKey() const;


		std::map<std::string, SDL_Scancode> m_KeyMap;
		static  std::map<std::string, SDL_Scancode> s_DEFAULT_KEY_MAP;

		Config();

		~Config();
	private:

	};
}


#endif //WINTHER_ENGINE_CONFIG_H
