#include "core/Engine.h"
#include "Log.h"
#include "entity/Entity.h"
#include "entity/EntityManager.h"
#include "graphics/MapParser.h"
#include "graphics/MapDocument.h"
#include "graphics/TextureManager.h"
#include "input/InputHandler.h"
#include "time/Time.h"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

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

	bool Engine::Init()
	{
		Log::Init();

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

		SDL_SetRenderVSync(m_Renderer, 1);

		if (!InitImGui())
		{
			Log::GetCoreLogger()->error("Failed to initialize ImGui.");
			SDL_DestroyRenderer(m_Renderer);
			SDL_DestroyWindow(m_Window);
			m_Renderer = nullptr;
			m_Window = nullptr;
			return false;
		}


		if (!LoadResources())
		{
			Log::GetCoreLogger()->error("Failed to load the initial project resources.");
			Quit();
			return false;
		}

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
		while (m_IsRunning)
		{
			m_IsRunning = ProcessEvents();
			if (!m_IsRunning)
			{
				break;
			}

			Time::GetInstance()->CalculateTickRate();
			Update();
			Render();
		}
	}

	bool Engine::ProcessEvents()
	{
		return InputHandler::GetInstance()->ProcessEvents();
	}

	void Engine::Update()
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if (m_ActiveDocument.has_value())
		{
			m_EditorUI.Draw(
				m_EditorState, *m_ActiveDocument, *TextureManager::GetInstance());
		}

		const float deltaTime = Time::GetInstance()->GetDeltaTime();
		if (m_ActiveDocument.has_value())
		{
			m_ActiveDocument->map.Update();
		}

		if (m_EditorState.mode == EditorMode::Play && !ImGui::GetIO().WantCaptureKeyboard)
		{
			EntityManager::GetInstance()->Update(deltaTime);
		}
		else if (m_EditorState.mode == EditorMode::Edit && m_ActiveDocument.has_value())
		{
			int windowWidth = 0;
			int windowHeight = 0;
			if (SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight))
			{
				const MapViewport viewport{
					0.0F,
					0.0F,
					static_cast<float>(windowWidth),
					static_cast<float>(windowHeight)};
				m_EditorController.Update(m_EditorState, *m_ActiveDocument, viewport);
			}
		}
	}

	void Engine::Render()
	{
		SDL_RenderClear(m_Renderer);
		if (m_ActiveDocument.has_value())
		{
			m_ActiveDocument->map.Render();
		}

		EntityManager::GetInstance()->Draw();

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}

	void Engine::Quit()
	{
		m_IsRunning = false;

		EntityManager::GetInstance()->Clean();
		MapParser::GetInstance()->Clean();
		TextureManager::GetInstance()->Clean();
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

	bool Engine::LoadResources()
	{
		Properties playerProperties(SDL_FLIP_NONE, 35, 45, 64, 64, "player", "sprites");
		if (!TextureManager::GetInstance()->Load("player", playerProperties.m_Source))
		{
			return false;
		}
		EntityManager::GetInstance()->AddEntity("player", playerProperties);

		m_ProjectContext.projectRoot = GetResourcePath({});
		m_ProjectContext.assetRoot = m_ProjectContext.projectRoot;
		m_ProjectContext.activeMapPath = m_ProjectContext.assetRoot / "maps" / "map.tmx";

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
}
