#include "game/GameSession.h"

#include <algorithm>
#include <cmath>

namespace Modwin
{
	ModwinGameApi GameSession::CreateApi() noexcept
	{
		ModwinGameApi api{};
		api.abi_version = MODWIN_GAME_ABI_VERSION;
		api.struct_size = static_cast<uint32_t>(sizeof(ModwinGameApi));
		api.game_context = this;
		api.initialize = &GameSession::InitializeCallback;
		api.shutdown = &GameSession::ShutdownCallback;
		api.enter_play = &GameSession::EnterPlayCallback;
		api.exit_play = &GameSession::ExitPlayCallback;
		api.capture_input = &GameSession::CaptureInputCallback;
		api.fixed_update = &GameSession::FixedUpdateCallback;
		api.update = &GameSession::UpdateCallback;
		return api;
	}

	ModwinResult GameSession::Initialize(const ModwinEngineApi* engineApi) noexcept
	{
		if (engineApi == nullptr ||
			engineApi->abi_version != MODWIN_GAME_ABI_VERSION ||
			engineApi->struct_size < MODWIN_ENGINE_API_V1_SIZE ||
			engineApi->load_texture == nullptr ||
			engineApi->create_entity == nullptr ||
			engineApi->destroy_entity == nullptr ||
			engineApi->input_key_down == nullptr ||
			engineApi->input_key_pressed == nullptr ||
			engineApi->submit_entity_commands == nullptr)
		{
			return MODWIN_RESULT_INCOMPATIBLE_ABI;
		}

		m_Engine = engineApi;
		const ModwinResult textureResult = m_Engine->load_texture(
			m_Engine->engine_context, "player", "sprites");
		if (textureResult != MODWIN_RESULT_OK)
		{
			m_Engine = nullptr;
		}
		return textureResult;
	}

	ModwinResult GameSession::Shutdown() noexcept
	{
		const ModwinResult exitResult = ExitPlay();
		m_Engine = nullptr;
		return exitResult;
	}

	ModwinResult GameSession::EnterPlay(const ModwinMapInfo* mapInfo) noexcept
	{
		if (m_Engine == nullptr || mapInfo == nullptr ||
			mapInfo->struct_size < MODWIN_MAP_INFO_V1_SIZE ||
			mapInfo->width <= 0 || mapInfo->height <= 0 ||
			mapInfo->tile_width <= 0 || mapInfo->tile_height <= 0)
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}

		const ModwinResult exitResult = ExitPlay();
		if (exitResult != MODWIN_RESULT_OK)
		{
			return exitResult;
		}

		ModwinEntityCreateInfo createInfo{};
		createInfo.struct_size = static_cast<uint32_t>(sizeof(ModwinEntityCreateInfo));
		createInfo.flags = MODWIN_ENTITY_HAS_KINEMATIC_BODY;
		createInfo.position = {35.0F, 45.0F};
		createInfo.sprite_size = {64.0F, 64.0F};
		createInfo.maximum_speed = 180.0F;
		createInfo.acceleration = 1200.0F;
		createInfo.deceleration = 1600.0F;
		createInfo.flip_mode = MODWIN_FLIP_NONE;
		createInfo.texture_id = "player";
		m_Player = m_Engine->create_entity(m_Engine->engine_context, &createInfo);
		return m_Player != MODWIN_INVALID_ENTITY_HANDLE
			? MODWIN_RESULT_OK
			: MODWIN_RESULT_INTERNAL_ERROR;
	}

	ModwinResult GameSession::ExitPlay() noexcept
	{
		ModwinResult result = MODWIN_RESULT_OK;
		if (m_Engine != nullptr && m_Player != MODWIN_INVALID_ENTITY_HANDLE)
		{
			result = m_Engine->destroy_entity(m_Engine->engine_context, m_Player);
			if (result == MODWIN_RESULT_NOT_FOUND)
			{
				result = MODWIN_RESULT_OK;
			}
		}
		m_Player = MODWIN_INVALID_ENTITY_HANDLE;
		m_PendingInput = {};
		m_FacingX = 0.0F;
		m_FacingY = 1.0F;
		return result;
	}

	ModwinResult GameSession::CaptureInput(const bool acceptPlayerInput) noexcept
	{
		if (m_Engine == nullptr)
		{
			return MODWIN_RESULT_INTERNAL_ERROR;
		}
		if (!acceptPlayerInput)
		{
			m_PendingInput = {};
			return MODWIN_RESULT_OK;
		}

		const auto down = [this](const GameAction action)
		{
			return m_Engine->input_key_down(
				m_Engine->engine_context, m_Bindings.Get(action)) != 0U;
		};
		m_PendingInput.movementX =
			(down(GameAction::MoveRight) ? 1.0F : 0.0F) -
			(down(GameAction::MoveLeft) ? 1.0F : 0.0F);
		m_PendingInput.movementY =
			(down(GameAction::MoveDown) ? 1.0F : 0.0F) -
			(down(GameAction::MoveUp) ? 1.0F : 0.0F);
		m_PendingInput.attackPressed = m_PendingInput.attackPressed ||
			m_Engine->input_key_pressed(
				m_Engine->engine_context, m_Bindings.Get(GameAction::Attack)) != 0U;
		return MODWIN_RESULT_OK;
	}

	ModwinResult GameSession::FixedUpdate(const float deltaSeconds) noexcept
	{
		if (m_Engine == nullptr || m_Player == MODWIN_INVALID_ENTITY_HANDLE ||
			!std::isfinite(deltaSeconds) || deltaSeconds <= 0.0F)
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}

		const float inputLengthSquared =
			m_PendingInput.movementX * m_PendingInput.movementX +
			m_PendingInput.movementY * m_PendingInput.movementY;
		if (inputLengthSquared > 0.0F)
		{
			const float inverseLength = 1.0F / std::sqrt(inputLengthSquared);
			m_FacingX = m_PendingInput.movementX * inverseLength;
			m_FacingY = m_PendingInput.movementY * inverseLength;
		}

		const bool moving = inputLengthSquared > 0.0F;
		const bool horizontal = std::abs(m_FacingX) > std::abs(m_FacingY);
		uint32_t animationRow = 2U;
		if (horizontal)
		{
			animationRow = m_FacingX > 0.0F ? 3U : 1U;
		}
		else if (m_FacingY < 0.0F)
		{
			animationRow = 0U;
		}

		ModwinEntityCommand commands[2]{};
		commands[0].struct_size = static_cast<uint32_t>(sizeof(ModwinEntityCommand));
		commands[0].type = MODWIN_COMMAND_SET_KINEMATIC_INPUT;
		commands[0].entity = m_Player;
		commands[0].value0 = m_PendingInput.movementX;
		commands[0].value1 = m_PendingInput.movementY;

		commands[1].struct_size = static_cast<uint32_t>(sizeof(ModwinEntityCommand));
		commands[1].type = MODWIN_COMMAND_SET_ANIMATION;
		commands[1].entity = m_Player;
		commands[1].data0 = animationRow;
		commands[1].data1 = moving ? 9U : 2U;
		commands[1].data2 = moving ? 80U : 250U;

		const ModwinResult result = m_Engine->submit_entity_commands(
			m_Engine->engine_context, commands, 2U);
		if (result == MODWIN_RESULT_OK)
		{
			m_PendingInput.attackPressed = false;
		}
		return result;
	}

	ModwinResult GameSession::Update(const float deltaSeconds) noexcept
	{
		return std::isfinite(deltaSeconds) && deltaSeconds >= 0.0F
			? MODWIN_RESULT_OK
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::InitializeCallback(
		void* context, const ModwinEngineApi* engineApi) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->Initialize(engineApi)
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::ShutdownCallback(void* context) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->Shutdown()
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::EnterPlayCallback(
		void* context, const ModwinMapInfo* mapInfo) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->EnterPlay(mapInfo)
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::ExitPlayCallback(void* context) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->ExitPlay()
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::CaptureInputCallback(
		void* context, const uint8_t acceptPlayerInput) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->CaptureInput(acceptPlayerInput != 0U)
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::FixedUpdateCallback(
		void* context, const float deltaSeconds) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->FixedUpdate(deltaSeconds)
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinResult MODWIN_ABI_CALL GameSession::UpdateCallback(
		void* context, const float deltaSeconds) noexcept
	{
		return context != nullptr
			? static_cast<GameSession*>(context)->Update(deltaSeconds)
			: MODWIN_RESULT_INVALID_ARGUMENT;
	}
}
