#pragma once
#include "editor/EditorState.h"

struct SDL_Renderer;
struct SDL_Window;

namespace Modwin
{
	class TileMap;

	class Engine
	{
		public:
			Engine(const Engine&) = delete;

			Engine& operator=(const Engine&) = delete;

			static Engine* GetInstance()
			{
				static Engine instance;
				return &instance;
			}

			bool Init();

			void Run();

			void Quit();

			[[nodiscard]] bool IsRunning() const noexcept;

			[[nodiscard]] SDL_Renderer* GetRenderer() const noexcept;

			~Engine() = default;

		private:
			Engine() = default;

			bool InitImGui();

			bool ProcessEvents();

			void LoadResources();

			void Update();

			void Render();

			EditorMode m_EditorMode;
			SDL_Renderer* m_Renderer = nullptr;
			SDL_Window* m_Window = nullptr;
			TileMap* m_CurrentLevel = nullptr;
			bool m_IsRunning = false;
	};
}
