//
// Created by komvu on 2025-01-03.
//

#include "cfg/Config.h"

namespace Modwin
{
	std::map<std::string, SDL_Scancode> Config::s_DEFAULT_KEY_MAP;

	Config::Config()
	{
		s_DEFAULT_KEY_MAP.insert(std::pair<std::string, SDL_Scancode>("MOVE_UP", SDL_SCANCODE_W));
		s_DEFAULT_KEY_MAP.insert(std::pair<std::string, SDL_Scancode>("MOVE_RIGHT", SDL_SCANCODE_D));
		s_DEFAULT_KEY_MAP.insert(std::pair<std::string, SDL_Scancode>("ATTACK", SDL_SCANCODE_SPACE));
		s_DEFAULT_KEY_MAP.insert(std::pair<std::string, SDL_Scancode>("MOVE_LEFT", SDL_SCANCODE_A));
		s_DEFAULT_KEY_MAP.insert(std::pair<std::string, SDL_Scancode>("MOVE_DOWN", SDL_SCANCODE_S));
		m_KeyMap.insert(s_DEFAULT_KEY_MAP.begin(), s_DEFAULT_KEY_MAP.end());
	}

	void Config::SetMoveUpKey(SDL_Scancode sdlKey)
	{
		m_KeyMap.insert(std::pair<std::string, SDL_Scancode>("MOVE_UP", sdlKey));
	}

	void Config::SetMoveRightKey(SDL_Scancode sdlKey)
	{
		m_KeyMap.insert(std::pair<std::string, SDL_Scancode>("MOVE_RIGHT", sdlKey));
	}

	void Config::SetMoveLeftKey(SDL_Scancode sdlKey)
	{
		m_KeyMap.insert(std::pair<std::string, SDL_Scancode>("MOVE_BACK", sdlKey));
	}

	void Config::SetAttackKey(SDL_Scancode sdlKey)
	{
		m_KeyMap.insert(std::pair<std::string, SDL_Scancode>("ATTACK", sdlKey));

	}

	void Config::SetMoveDownKey(SDL_Scancode sdlKey)
	{
		m_KeyMap.insert(std::pair<std::string, SDL_Scancode>("MOVE_DOWN", sdlKey));
	}

	SDL_Scancode Config::GetMoveUpKey()
	{
		return m_KeyMap.at("MOVE_UP");
	}

	SDL_Scancode Config::GetMoveRightKey()
	{
		return m_KeyMap.at("MOVE_RIGHT");
	}

	SDL_Scancode Config::GetMoveLeftKey()
	{
		return m_KeyMap.at("MOVE_LEFT");
	}

	SDL_Scancode Config::GetAttackKey()
	{
		return m_KeyMap.at("ATTACK");
	}

	SDL_Scancode Config::GetMoveDownKey()
	{
		return m_KeyMap.at("MOVE_DOWN");
	}

}
