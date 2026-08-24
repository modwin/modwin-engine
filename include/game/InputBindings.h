#ifndef MODWIN_GAME_INPUTBINDINGS_H
#define MODWIN_GAME_INPUTBINDINGS_H

#include "modwin/abi/ModwinGameApi.h"

#include <array>
#include <cstddef>

namespace Modwin
{
	enum class GameAction : std::size_t
	{
		MoveUp,
		MoveRight,
		MoveDown,
		MoveLeft,
		Attack,
		Count
	};

	class InputBindings
	{
	public:
		InputBindings() noexcept
			: m_Keys{
				MODWIN_KEY_W,
				MODWIN_KEY_D,
				MODWIN_KEY_S,
				MODWIN_KEY_A,
				MODWIN_KEY_SPACE}
		{
		}

		void Set(const GameAction action, const uint32_t key) noexcept
		{
			m_Keys[static_cast<std::size_t>(action)] = key;
		}

		[[nodiscard]] uint32_t Get(const GameAction action) const noexcept
		{
			return m_Keys[static_cast<std::size_t>(action)];
		}

	private:
		std::array<uint32_t, static_cast<std::size_t>(GameAction::Count)> m_Keys;
	};
}

#endif // MODWIN_GAME_INPUTBINDINGS_H
