#ifndef MODWIN_ENGINE_SPRITE_H
#define MODWIN_ENGINE_SPRITE_H
#include "modwin/abi/ModwinGameApi.h"

namespace Modwin
{
	enum class SpriteType
	{
		Player,
		Collectible,
		Hazard,
		Enemy,
		Trigger
	};

	class Sprite
	{
		public:
			Sprite() = default;
			virtual ~Sprite() = default;

			Sprite(const Sprite&) = delete;
			Sprite& operator=(const Sprite&) = delete;

			virtual void Update(float deltaSeconds, const ModwinEngineApi& engine) = 0;
			[[nodiscard]] SpriteType GetType() const noexcept { return m_Type; }
			[[nodiscard]] ModwinEntityHandle GetHandle() const noexcept { return m_Handle; }
			[[nodiscard]] bool IsPendingDestroy() const noexcept { return m_PendingDestroy; }


		protected:
			SpriteType m_Type;
			bool m_PendingDestroy = false;
			ModwinEntityHandle m_Handle = MODWIN_INVALID_ENTITY_HANDLE;
	};
}
#endif //MODWIN_ENGINE_SPRITE_H
