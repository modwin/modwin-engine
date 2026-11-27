#ifndef MODWIN_ENGINE_SPRITE_H
#define MODWIN_ENGINE_SPRITE_H
#include "modwin/abi/ModwinGameApi.h"

namespace Modwin
{
	class Sprite
	{
		public:
			Sprite() = default;
			virtual ~Sprite() = default;

			Sprite(const Sprite&) = delete;
			Sprite& operator=(const Sprite&) = delete;

			virtual void Update(float deltaSeconds) = 0;
			[[nodiscard]] ModwinEntityHandle GetHandle() const noexcept { return m_Handle; }

		protected:
			ModwinEntityHandle m_Handle = MODWIN_INVALID_ENTITY_HANDLE;
	};
}
#endif //MODWIN_ENGINE_SPRITE_H
