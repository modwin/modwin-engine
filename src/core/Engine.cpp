#include "core/Engine.h"
#include "Log.h"
#include "entity/Entity.h"
#include "entity/EntityManager.h"
#include "graphics/MapParser.h"
#include "graphics/TextureManager.h"
#include "graphics/TileMap.h"
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


		LoadResources();
		m_IsRunning = true;
		m_EditorMode = EditorMode::Play;
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
		ImGui::ShowDemoWindow();

		const float deltaTime = Time::GetInstance()->GetDeltaTime();
		if (m_CurrentLevel != nullptr)
		{
			m_CurrentLevel->Update();
		}
		if (m_EditorMode == EditorMode::Play)
		{
			EntityManager::GetInstance()->Update(deltaTime);
		}
		else
		{
			std::cout << "Editor::Update()" << std::endl;
		}
	}

	void Engine::Render()
	{
		SDL_RenderClear(m_Renderer);
		if (m_CurrentLevel != nullptr)
		{
			m_CurrentLevel->Render();
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
		m_CurrentLevel = nullptr;

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

	void Engine::LoadResources()
	{
		Properties playerProperties(SDL_FLIP_NONE, 35, 45, 64, 64, "player", "sprites");
		TextureManager::GetInstance()->Load("player", playerProperties.m_Source);
		EntityManager::GetInstance()->AddEntity("player", playerProperties);

		constexpr auto mapId = "main";
		if (!MapParser::GetInstance()->LoadMap(mapId, "map"))
		{
			Log::GetCoreLogger()->error("Failed to load the map.");
			return;
		}

		m_CurrentLevel = MapParser::GetInstance()->GetTileMap(mapId);
	}
}
