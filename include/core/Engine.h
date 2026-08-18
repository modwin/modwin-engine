#pragma once
#include "editor/EditorState.h"
#include "editor/EditorController.h"
#include "editor/EditorUI.h"
#include "graphics/MapDocument.h"
#include "core/ResourcePaths.h"

#include <optional>

struct SDL_Renderer;
struct SDL_Window;

namespace Modwin
{
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

			bool LoadResources();

			void Update();

			void Render();

			ProjectContext m_ProjectContext;
			std::optional<MapDocument> m_ActiveDocument;
			EditorState m_EditorState;
			EditorUI m_EditorUI;
			EditorController m_EditorController;
			SDL_Renderer* m_Renderer = nullptr;
			SDL_Window* m_Window = nullptr;
			bool m_IsRunning = false;
	};
}
