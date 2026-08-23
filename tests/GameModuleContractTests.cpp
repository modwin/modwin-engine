#include "game/GameSession.h"

#include <cmath>
#include <cstdint>
#include <iostream>

namespace
{
	struct FakeEngine
	{
		bool textureLoaded = false;
		bool entityCreated = false;
		bool entityDestroyed = false;
		ModwinEntityCommand commands[2]{};
		uint32_t commandCount = 0;
	};

	ModwinResult MODWIN_ABI_CALL LoadTexture(
		void* context, const char* textureId, const char* directory)
	{
		auto& engine = *static_cast<FakeEngine*>(context);
		engine.textureLoaded = textureId != nullptr && directory != nullptr;
		return engine.textureLoaded ? MODWIN_RESULT_OK : MODWIN_RESULT_INVALID_ARGUMENT;
	}

	ModwinEntityHandle MODWIN_ABI_CALL CreateEntity(
		void* context, const ModwinEntityCreateInfo* createInfo)
	{
		auto& engine = *static_cast<FakeEngine*>(context);
		engine.entityCreated = createInfo != nullptr &&
			createInfo->struct_size >= MODWIN_ENTITY_CREATE_INFO_V1_SIZE &&
			(createInfo->flags & MODWIN_ENTITY_HAS_KINEMATIC_BODY) != 0U;
		return engine.entityCreated ? UINT64_C(42) : MODWIN_INVALID_ENTITY_HANDLE;
	}

	ModwinResult MODWIN_ABI_CALL DestroyEntity(
		void* context, const ModwinEntityHandle entity)
	{
		auto& engine = *static_cast<FakeEngine*>(context);
		engine.entityDestroyed = entity == UINT64_C(42);
		return engine.entityDestroyed ? MODWIN_RESULT_OK : MODWIN_RESULT_NOT_FOUND;
	}

	uint8_t MODWIN_ABI_CALL KeyDown(void*, const uint32_t keyCode)
	{
		return keyCode == MODWIN_KEY_D ? 1U : 0U;
	}

	uint8_t MODWIN_ABI_CALL KeyPressed(void*, const uint32_t keyCode)
	{
		return keyCode == MODWIN_KEY_SPACE ? 1U : 0U;
	}

	ModwinResult MODWIN_ABI_CALL SubmitCommands(
		void* context, const ModwinEntityCommand* commands, const uint32_t commandCount)
	{
		if (commands == nullptr || commandCount > 2U)
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}
		auto& engine = *static_cast<FakeEngine*>(context);
		engine.commandCount = commandCount;
		for (uint32_t index = 0; index < commandCount; ++index)
		{
			engine.commands[index] = commands[index];
		}
		return MODWIN_RESULT_OK;
	}

	bool NearlyEqual(const float left, const float right)
	{
		return std::abs(left - right) < 0.001F;
	}

	bool Check(const bool condition, const char* message)
	{
		if (!condition)
		{
			std::cerr << "FAILED: " << message << '\n';
		}
		return condition;
	}
}

int main()
{
	FakeEngine fakeEngine;
	ModwinEngineApi engineApi{};
	engineApi.abi_version = MODWIN_GAME_ABI_VERSION;
	engineApi.struct_size = static_cast<uint32_t>(sizeof(ModwinEngineApi));
	engineApi.engine_context = &fakeEngine;
	engineApi.input_key_down = &KeyDown;
	engineApi.input_key_pressed = &KeyPressed;
	engineApi.load_texture = &LoadTexture;
	engineApi.create_entity = &CreateEntity;
	engineApi.destroy_entity = &DestroyEntity;
	engineApi.submit_entity_commands = &SubmitCommands;

	Modwin::GameSession gameSession;
	const ModwinGameApi gameApi = gameSession.CreateApi();
	bool passed = true;
	passed &= Check(
		gameApi.initialize(gameApi.game_context, &engineApi) == MODWIN_RESULT_OK &&
		fakeEngine.textureLoaded,
		"The game module should load its texture through the engine API.");

	const ModwinMapInfo mapInfo{
		static_cast<uint32_t>(sizeof(ModwinMapInfo)), 20, 10, 32, 32};
	passed &= Check(
		gameApi.enter_play(gameApi.game_context, &mapInfo) == MODWIN_RESULT_OK &&
		fakeEngine.entityCreated,
		"Entering Play mode should create the player through an opaque handle.");
	passed &= Check(
		gameApi.capture_input(gameApi.game_context, 1U) == MODWIN_RESULT_OK &&
		gameApi.fixed_update(gameApi.game_context, 1.0F / 60.0F) == MODWIN_RESULT_OK,
		"The game module should accept input and complete a fixed update.");
	passed &= Check(
		fakeEngine.commandCount == 2U &&
		fakeEngine.commands[0].type == MODWIN_COMMAND_SET_KINEMATIC_INPUT &&
		NearlyEqual(fakeEngine.commands[0].value0, 1.0F) &&
		NearlyEqual(fakeEngine.commands[0].value1, 0.0F),
		"Gameplay movement should be submitted as a batched engine command.");
	passed &= Check(
		fakeEngine.commands[1].type == MODWIN_COMMAND_SET_ANIMATION &&
		fakeEngine.commands[1].data0 == 3U &&
		fakeEngine.commands[1].data1 == 9U,
		"The native game module should select the right-facing walk animation.");
	passed &= Check(
		gameApi.exit_play(gameApi.game_context) == MODWIN_RESULT_OK &&
		fakeEngine.entityDestroyed,
		"Leaving Play mode should release the opaque player entity.");
	passed &= Check(
		gameApi.shutdown(gameApi.game_context) == MODWIN_RESULT_OK,
		"The game module should shut down cleanly after leaving Play mode.");

	return passed ? 0 : 1;
}
