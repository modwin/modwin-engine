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

	bool Engine::Init(ProjectContext projectContext)
	{
		Log::Init();
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

	void Engine::Update()
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
				EntityManager::GetInstance()->Draw();
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
