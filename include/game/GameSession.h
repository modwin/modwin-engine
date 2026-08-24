#ifndef MODWIN_GAME_GAMESESSION_H
#define MODWIN_GAME_GAMESESSION_H

#include "game/InputBindings.h"
#include "modwin/abi/ModwinGameApi.h"

namespace Modwin
{
	// Native reference game module. It deliberately depends only on the public C
	// ABI so that managed-language adapters can implement the same contract.
	class GameSession final
	{
	public:
		[[nodiscard]] ModwinGameApi CreateApi() noexcept;

	private:
		struct PlayerInput
		{
			float movementX = 0.0F;
			float movementY = 0.0F;
			bool attackPressed = false;
		};

		ModwinResult Initialize(const ModwinEngineApi* engineApi) noexcept;
		ModwinResult Shutdown() noexcept;
		ModwinResult EnterPlay(const ModwinMapInfo* mapInfo) noexcept;
		ModwinResult ExitPlay() noexcept;
		ModwinResult CaptureInput(bool acceptPlayerInput) noexcept;
		ModwinResult FixedUpdate(float deltaSeconds) noexcept;
		ModwinResult Update(float deltaSeconds) noexcept;

		static ModwinResult MODWIN_ABI_CALL InitializeCallback(
			void* context, const ModwinEngineApi* engineApi) noexcept;
		static ModwinResult MODWIN_ABI_CALL ShutdownCallback(void* context) noexcept;
		static ModwinResult MODWIN_ABI_CALL EnterPlayCallback(
			void* context, const ModwinMapInfo* mapInfo) noexcept;
		static ModwinResult MODWIN_ABI_CALL ExitPlayCallback(void* context) noexcept;
		static ModwinResult MODWIN_ABI_CALL CaptureInputCallback(
			void* context, uint8_t acceptPlayerInput) noexcept;
		static ModwinResult MODWIN_ABI_CALL FixedUpdateCallback(
			void* context, float deltaSeconds) noexcept;
		static ModwinResult MODWIN_ABI_CALL UpdateCallback(
			void* context, float deltaSeconds) noexcept;

		const ModwinEngineApi* m_Engine = nullptr;
		InputBindings m_Bindings;
		ModwinEntityHandle m_Player = MODWIN_INVALID_ENTITY_HANDLE;
		PlayerInput m_PendingInput{};
		float m_FacingX = 0.0F;
		float m_FacingY = 1.0F;
	};
}

#endif // MODWIN_GAME_GAMESESSION_H
