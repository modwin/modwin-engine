#include "core/Engine.h"

#include "Log.h"
#include "graphics/MapDocument.h"
#include "graphics/MapParser.h"
#include "graphics/TextureManager.h"
#include "input/InputHandler.h"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace Modwin
{
	SDL_Renderer* Engine::GetRenderer() const noexcept
	{
		return m_Renderer;
	}

	bool Engine::IsRunning() const noexcept
	{
		return m_IsRunning;
	}

	bool Engine::Init(
		ModwinGameApi gameApi,
		ProjectContext projectContext)
	{
		Log::Init();
		if (!ValidateGameApi(gameApi))
		{
			return false;
		}
		m_GameApi = gameApi;
		BuildEngineApi();
		m_ProjectContext = projectContext.activeMapPath.empty()
			? CreateDefaultProjectContext()
			: std::move(projectContext);

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			Log::GetCoreLogger()->error("Failed to initialize SDL: {}", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Modwin Engine", 1280, 720, SDL_WINDOW_RESIZABLE);
		if (m_Window == nullptr)
		{
			Log::GetCoreLogger()->error("Failed to create the window: {}", SDL_GetError());
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
		if (m_Renderer == nullptr)
		{
			Log::GetCoreLogger()->error("Failed to create the renderer: {}", SDL_GetError());
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
			return false;
		}
		TextureManager::GetInstance()->SetRenderer(m_Renderer);

		SDL_SetRenderVSync(m_Renderer, 1);

		if (!InitImGui())
		{
			Log::GetCoreLogger()->error("Failed to initialize ImGui.");
			TextureManager::GetInstance()->SetRenderer(nullptr);
			SDL_DestroyRenderer(m_Renderer);
			SDL_DestroyWindow(m_Window);
			m_Renderer = nullptr;
			m_Window = nullptr;
			return false;
		}

		if (!LoadProjectResources())
		{
			Log::GetCoreLogger()->error("Failed to load the initial project resources.");
			Quit();
			return false;
		}

		if (m_GameApi.initialize(m_GameApi.game_context, &m_EngineApi) != MODWIN_RESULT_OK)
		{
			Log::GetCoreLogger()->error("The game module failed to initialize.");
			Quit();
			return false;
		}
		m_GameInitialized = true;

		m_IsRunning = true;
		Log::GetCoreLogger()->info("Modwin Engine initialized successfully.");
		return true;
	}

	bool Engine::InitImGui()
	{
		IMGUI_CHECKVERSION();
		if (ImGui::CreateContext() == nullptr)
		{
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		if (!ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer))
		{
			ImGui::DestroyContext();
			return false;
		}

		if (!ImGui_ImplSDLRenderer3_Init(m_Renderer))
		{
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();
			return false;
		}

		return true;
	}

	void Engine::Run()
	{
		m_FrameClock.Reset();
		while (m_IsRunning)
		{
			m_IsRunning = ProcessEvents();
			if (!m_IsRunning)
			{
				break;
			}

			Update(m_FrameClock.Tick());
			Render();
		}
	}

	bool Engine::ProcessEvents()
	{
		auto* input = InputHandler::GetInstance();
		input->ProcessEvents();
		if (!input->ConsumeQuitRequest())
		{
			return true;
		}

		if (m_ActiveDocument.has_value() && m_ActiveDocument->IsDirty())
		{
			m_CloseConfirmationRequested = true;
			return true;
		}

		return false;
	}

	void Engine::Update(const float frameDeltaSeconds)
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if (m_ActiveDocument.has_value())
		{
			m_MapViewport = m_EditorUI.Draw(
				m_EditorState, *m_ActiveDocument, *TextureManager::GetInstance());
			ClampEditorCamera(
				m_EditorState.camera,
				m_MapViewport,
				static_cast<float>(m_ActiveDocument->map.GetWidth() *
					m_ActiveDocument->map.GetTileWidth()),
				static_cast<float>(m_ActiveDocument->map.GetHeight() *
					m_ActiveDocument->map.GetTileHeight()));

			const CloseDecision closeDecision = m_EditorUI.DrawCloseConfirmation(
				*m_ActiveDocument, m_CloseConfirmationRequested);
			if (closeDecision == CloseDecision::SaveAndClose ||
				closeDecision == CloseDecision::DiscardAndClose)
			{
				m_IsRunning = false;
				m_CloseConfirmationRequested = false;
			}
			else if (closeDecision == CloseDecision::Cancel)
			{
				m_CloseConfirmationRequested = false;
			}
		}

		if (m_ActiveDocument.has_value())
		{
			m_ActiveDocument->map.Update();
		}

		HandlePlayModeTransition();
		if (!m_IsRunning)
		{
			return;
		}
		if (m_EditorState.mode == EditorMode::Play)
		{
			if (!HandleGameResult(
				m_GameApi.capture_input(
					m_GameApi.game_context,
					ImGui::GetIO().WantCaptureKeyboard ? 0U : 1U),
				"capture input"))
			{
				return;
			}

			const float maximumAccumulatedTime =
				FrameClock::FixedDeltaSeconds *
				static_cast<float>(FrameClock::MaximumFixedStepsPerFrame);
			m_FixedStepAccumulator = std::min(
				m_FixedStepAccumulator + frameDeltaSeconds,
				maximumAccumulatedTime);

			int fixedSteps = 0;
			while (m_FixedStepAccumulator >= FrameClock::FixedDeltaSeconds &&
				fixedSteps < FrameClock::MaximumFixedStepsPerFrame)
			{
				if (!HandleGameResult(
					m_GameApi.fixed_update(
						m_GameApi.game_context, FrameClock::FixedDeltaSeconds),
					"fixed update"))
				{
					return;
				}
				m_RuntimeWorld.FixedUpdate(FrameClock::FixedDeltaSeconds);
				m_FixedStepAccumulator -= FrameClock::FixedDeltaSeconds;
				++fixedSteps;
			}
			if (!HandleGameResult(
				m_GameApi.update(m_GameApi.game_context, frameDeltaSeconds),
				"variable update"))
			{
				return;
			}
			m_RuntimeWorld.Update(frameDeltaSeconds);
		}
		else if (m_ActiveDocument.has_value())
		{
			m_EditorController.Update(m_EditorState, *m_ActiveDocument, m_MapViewport);
		}
	}

	void Engine::Render()
	{
		SDL_SetRenderDrawColor(m_Renderer, 24, 26, 32, 255);
		SDL_RenderClear(m_Renderer);
		if (m_ActiveDocument.has_value() &&
			m_MapViewport.width > 0.0F && m_MapViewport.height > 0.0F)
		{
			const SDL_Rect viewportRect{
				static_cast<int>(m_MapViewport.x),
				static_cast<int>(m_MapViewport.y),
				static_cast<int>(m_MapViewport.width),
				static_cast<int>(m_MapViewport.height)};
			SDL_SetRenderViewport(m_Renderer, &viewportRect);

			const bool editing = m_EditorState.mode == EditorMode::Edit;
			const float zoom = editing ? m_EditorState.camera.zoom : 1.0F;
			const float cameraX = editing ? m_EditorState.camera.x : 0.0F;
			const float cameraY = editing ? m_EditorState.camera.y : 0.0F;
			SDL_SetRenderScale(m_Renderer, zoom, zoom);
			m_ActiveDocument->map.Render(cameraX, cameraY);

			if (editing && m_EditorState.hoveredColumn >= 0 && m_EditorState.hoveredRow >= 0)
			{
				const float tileWidth = static_cast<float>(m_ActiveDocument->map.GetTileWidth());
				const float tileHeight = static_cast<float>(m_ActiveDocument->map.GetTileHeight());
				const SDL_FRect highlight{
					static_cast<float>(m_EditorState.hoveredColumn) * tileWidth - cameraX,
					static_cast<float>(m_EditorState.hoveredRow) * tileHeight - cameraY,
					tileWidth,
					tileHeight};
				SDL_SetRenderDrawColor(m_Renderer, 255, 214, 64, 255);
				SDL_RenderRect(m_Renderer, &highlight);
			}
			else if (!editing)
			{
				m_RuntimeWorld.Render();
			}

			SDL_SetRenderScale(m_Renderer, 1.0F, 1.0F);
			SDL_SetRenderViewport(m_Renderer, nullptr);
		}

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}

	void Engine::Quit()
	{
		m_IsRunning = false;

		if (m_GameInitialized)
		{
			const ModwinResult result = m_GameApi.shutdown(m_GameApi.game_context);
			if (result != MODWIN_RESULT_OK)
			{
				Log::GetCoreLogger()->error(
					"The game module failed during shutdown (result {}).",
					static_cast<int>(result));
			}
			m_GameInitialized = false;
		}
		m_RuntimeWorld.Clear();
		m_GameApi = {};
		m_EngineApi = {};
		MapParser::GetInstance()->Clean();
		TextureManager::GetInstance()->Clean();
		TextureManager::GetInstance()->SetRenderer(nullptr);
		m_ActiveDocument.reset();

		if (ImGui::GetCurrentContext() != nullptr)
		{
			ImGui_ImplSDLRenderer3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();
		}

		if (m_Renderer != nullptr)
		{
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		}

		if (m_Window != nullptr)
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}

	bool Engine::LoadProjectResources()
	{
		auto tileMap = MapParser::LoadFromFile(m_ProjectContext.activeMapPath);
		if (!tileMap.has_value())
		{
			return false;
		}

		for (const auto& tileset : tileMap->GetTilesets())
		{
			const auto texturePath =
				(m_ProjectContext.activeMapPath.parent_path() / tileset.source).lexically_normal();
			if (!TextureManager::GetInstance()->LoadFromFile(tileset.name, texturePath))
			{
				return false;
			}
		}

		m_ActiveDocument.emplace(m_ProjectContext.activeMapPath, std::move(*tileMap));
		return true;
	}

	bool Engine::ValidateGameApi(const ModwinGameApi& gameApi) const
	{
		if (gameApi.abi_version != MODWIN_GAME_ABI_VERSION ||
			gameApi.struct_size < MODWIN_GAME_API_V1_SIZE)
		{
			Log::GetCoreLogger()->error(
				"The game module uses an incompatible ABI (version {}, size {}).",
				gameApi.abi_version,
				gameApi.struct_size);
			return false;
		}

		const bool hasRequiredCallbacks =
			gameApi.initialize != nullptr &&
			gameApi.shutdown != nullptr &&
			gameApi.enter_play != nullptr &&
			gameApi.exit_play != nullptr &&
			gameApi.capture_input != nullptr &&
			gameApi.fixed_update != nullptr &&
			gameApi.update != nullptr;
		if (!hasRequiredCallbacks)
		{
			Log::GetCoreLogger()->error("The game module is missing a required callback.");
		}
		return hasRequiredCallbacks;
	}

	void Engine::BuildEngineApi() noexcept
	{
		m_EngineApi = {};
		m_EngineApi.abi_version = MODWIN_GAME_ABI_VERSION;
		m_EngineApi.struct_size = static_cast<uint32_t>(sizeof(ModwinEngineApi));
		m_EngineApi.engine_context = this;
		m_EngineApi.log = &Engine::ApiLog;
		m_EngineApi.input_key_down = &Engine::ApiInputKeyDown;
		m_EngineApi.input_key_pressed = &Engine::ApiInputKeyPressed;
		m_EngineApi.input_key_released = &Engine::ApiInputKeyReleased;
		m_EngineApi.load_texture = &Engine::ApiLoadTexture;
		m_EngineApi.create_entity = &Engine::ApiCreateEntity;
		m_EngineApi.destroy_entity = &Engine::ApiDestroyEntity;
		m_EngineApi.get_entity_position = &Engine::ApiGetEntityPosition;
		m_EngineApi.submit_entity_commands = &Engine::ApiSubmitEntityCommands;
	}

	bool Engine::HandleGameResult(const ModwinResult result, const char* operation)
	{
		if (result == MODWIN_RESULT_OK)
		{
			return true;
		}
		Log::GetCoreLogger()->error(
			"The game module failed to {} (result {}).",
			operation,
			static_cast<int>(result));
		m_IsRunning = false;
		return false;
	}

	void MODWIN_ABI_CALL Engine::ApiLog(
		void* context, const uint32_t level, const char* message) noexcept
	{
		if (context == nullptr || message == nullptr)
		{
			return;
		}
		try
		{
			switch (level)
			{
			case MODWIN_LOG_TRACE:
				Log::GetClientLogger()->trace("{}", message);
				break;
			case MODWIN_LOG_WARNING:
				Log::GetClientLogger()->warn("{}", message);
				break;
			case MODWIN_LOG_ERROR:
				Log::GetClientLogger()->error("{}", message);
				break;
			case MODWIN_LOG_INFO:
			default:
				Log::GetClientLogger()->info("{}", message);
				break;
			}
		}
		catch (...)
		{
			// Exceptions must never cross the C ABI boundary.
		}
	}

	uint8_t MODWIN_ABI_CALL Engine::ApiInputKeyDown(
		void* context, const uint32_t keyCode) noexcept
	{
		return context != nullptr &&
			InputHandler::GetInstance()->IsKeyDown(static_cast<SDL_Scancode>(keyCode));
	}

	uint8_t MODWIN_ABI_CALL Engine::ApiInputKeyPressed(
		void* context, const uint32_t keyCode) noexcept
	{
		return context != nullptr &&
			InputHandler::GetInstance()->WasKeyPressed(static_cast<SDL_Scancode>(keyCode));
	}

	uint8_t MODWIN_ABI_CALL Engine::ApiInputKeyReleased(
		void* context, const uint32_t keyCode) noexcept
	{
		return context != nullptr &&
			InputHandler::GetInstance()->WasKeyReleased(static_cast<SDL_Scancode>(keyCode));
	}

	ModwinResult MODWIN_ABI_CALL Engine::ApiLoadTexture(
		void* context,
		const char* textureId,
		const char* resourceDirectory) noexcept
	{
		if (context == nullptr || textureId == nullptr || textureId[0] == '\0' ||
			resourceDirectory == nullptr || resourceDirectory[0] == '\0')
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}
		try
		{
			return TextureManager::GetInstance()->Load(textureId, resourceDirectory)
				? MODWIN_RESULT_OK
				: MODWIN_RESULT_NOT_FOUND;
		}
		catch (...)
		{
			return MODWIN_RESULT_INTERNAL_ERROR;
		}
	}

	ModwinEntityHandle MODWIN_ABI_CALL Engine::ApiCreateEntity(
		void* context, const ModwinEntityCreateInfo* createInfo) noexcept
	{
		if (context == nullptr || createInfo == nullptr ||
			createInfo->struct_size < MODWIN_ENTITY_CREATE_INFO_V1_SIZE ||
			(createInfo->flags & ~MODWIN_ENTITY_HAS_KINEMATIC_BODY) != 0U ||
			(createInfo->flip_mode & ~(MODWIN_FLIP_HORIZONTAL | MODWIN_FLIP_VERTICAL)) != 0U ||
			createInfo->texture_id == nullptr || createInfo->texture_id[0] == '\0' ||
			!std::isfinite(createInfo->position.x) ||
			!std::isfinite(createInfo->position.y) ||
			!std::isfinite(createInfo->sprite_size.x) ||
			!std::isfinite(createInfo->sprite_size.y) ||
			createInfo->sprite_size.x <= 0.0F || createInfo->sprite_size.y <= 0.0F ||
			!std::isfinite(createInfo->maximum_speed) || createInfo->maximum_speed < 0.0F ||
			!std::isfinite(createInfo->acceleration) || createInfo->acceleration < 0.0F ||
			!std::isfinite(createInfo->deceleration) || createInfo->deceleration < 0.0F)
		{
			return MODWIN_INVALID_ENTITY_HANDLE;
		}
		try
		{
			return static_cast<Engine*>(context)->m_RuntimeWorld.CreateEntity(*createInfo);
		}
		catch (...)
		{
			return MODWIN_INVALID_ENTITY_HANDLE;
		}
	}

	ModwinResult MODWIN_ABI_CALL Engine::ApiDestroyEntity(
		void* context, const ModwinEntityHandle entity) noexcept
	{
		if (context == nullptr || entity == MODWIN_INVALID_ENTITY_HANDLE)
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}
		return static_cast<Engine*>(context)->m_RuntimeWorld.DestroyEntity(entity)
			? MODWIN_RESULT_OK
			: MODWIN_RESULT_NOT_FOUND;
	}

	ModwinResult MODWIN_ABI_CALL Engine::ApiGetEntityPosition(
		void* context, const ModwinEntityHandle entity, ModwinVec2* position) noexcept
	{
		if (context == nullptr || entity == MODWIN_INVALID_ENTITY_HANDLE || position == nullptr)
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}
		Vector2D enginePosition;
		if (!static_cast<Engine*>(context)->m_RuntimeWorld.GetPosition(entity, enginePosition))
		{
			return MODWIN_RESULT_NOT_FOUND;
		}
		position->x = enginePosition.X;
		position->y = enginePosition.Y;
		return MODWIN_RESULT_OK;
	}

	ModwinResult MODWIN_ABI_CALL Engine::ApiSubmitEntityCommands(
		void* context,
		const ModwinEntityCommand* commands,
		const uint32_t commandCount) noexcept
	{
		if (context == nullptr || (commandCount > 0U && commands == nullptr))
		{
			return MODWIN_RESULT_INVALID_ARGUMENT;
		}

		Engine& engine = *static_cast<Engine*>(context);
		try
		{
			for (uint32_t index = 0; index < commandCount; ++index)
			{
				const ModwinEntityCommand& command = commands[index];
				if (command.struct_size < MODWIN_ENTITY_COMMAND_V1_SIZE ||
					command.entity == MODWIN_INVALID_ENTITY_HANDLE)
				{
					return MODWIN_RESULT_INVALID_ARGUMENT;
				}

				bool applied = false;
				switch (command.type)
				{
				case MODWIN_COMMAND_SET_KINEMATIC_INPUT:
					if (!std::isfinite(command.value0) || !std::isfinite(command.value1))
					{
						return MODWIN_RESULT_INVALID_ARGUMENT;
					}
					applied = engine.m_RuntimeWorld.SetKinematicInput(
						command.entity, {command.value0, command.value1});
					break;
				case MODWIN_COMMAND_SET_ANIMATION:
					if (command.data0 > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
						command.data1 == 0U ||
						command.data1 > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
						command.data2 == 0U ||
						command.data2 > static_cast<uint32_t>(std::numeric_limits<int>::max()))
					{
						return MODWIN_RESULT_INVALID_ARGUMENT;
					}
					applied = engine.m_RuntimeWorld.SetAnimation(
						command.entity,
						static_cast<int>(command.data0),
						static_cast<int>(command.data1),
						static_cast<int>(command.data2));
					break;
				case MODWIN_COMMAND_SET_POSITION:
					if (!std::isfinite(command.value0) || !std::isfinite(command.value1))
					{
						return MODWIN_RESULT_INVALID_ARGUMENT;
					}
					applied = engine.m_RuntimeWorld.SetPosition(
						command.entity, {command.value0, command.value1});
					break;
				default:
					return MODWIN_RESULT_INVALID_ARGUMENT;
				}

				if (!applied)
				{
					return MODWIN_RESULT_NOT_FOUND;
				}
			}
			return MODWIN_RESULT_OK;
		}
		catch (...)
		{
			return MODWIN_RESULT_INTERNAL_ERROR;
		}
	}

	void Engine::HandlePlayModeTransition()
	{
		if (m_EditorState.mode == m_PreviousEditorMode)
		{
			return;
		}

		m_FixedStepAccumulator = 0.0F;
		if (m_EditorState.mode == EditorMode::Play && m_ActiveDocument.has_value())
		{
			m_RuntimeWorld.Clear();
			const ModwinMapInfo mapInfo{
				static_cast<uint32_t>(sizeof(ModwinMapInfo)),
				m_ActiveDocument->map.GetWidth(),
				m_ActiveDocument->map.GetHeight(),
				m_ActiveDocument->map.GetTileWidth(),
				m_ActiveDocument->map.GetTileHeight()};
			HandleGameResult(
				m_GameApi.enter_play(m_GameApi.game_context, &mapInfo),
				"enter play mode");
		}
		else
		{
			HandleGameResult(
				m_GameApi.exit_play(m_GameApi.game_context),
				"exit play mode");
			m_RuntimeWorld.Clear();
		}
		m_PreviousEditorMode = m_EditorState.mode;
	}
}
