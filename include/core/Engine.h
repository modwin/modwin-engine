#pragma once

#include "core/ResourcePaths.h"
#include "editor/EditorController.h"
#include "editor/EditorState.h"
#include "editor/EditorUI.h"
#include "graphics/MapDocument.h"
#include "modwin/abi/ModwinGameApi.h"
#include "runtime/RuntimeWorld.h"
#include "time/Time.h"

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

		bool Init(
			ModwinGameApi gameApi,
			ProjectContext projectContext = {});
		void Run();
		void Quit();

		[[nodiscard]] bool IsRunning() const noexcept;
		[[nodiscard]] SDL_Renderer* GetRenderer() const noexcept;

		~Engine() = default;

	private:
		Engine() = default;

		bool InitImGui();
		bool ProcessEvents();
		bool LoadProjectResources();
		bool ValidateGameApi(const ModwinGameApi& gameApi) const;
		void BuildEngineApi() noexcept;
		bool HandleGameResult(ModwinResult result, const char* operation);
		void HandlePlayModeTransition();
		void Update(float frameDeltaSeconds);
		void Render();

		static void MODWIN_ABI_CALL ApiLog(
			void* context, uint32_t level, const char* message) noexcept;
		static uint8_t MODWIN_ABI_CALL ApiInputKeyDown(
			void* context, uint32_t keyCode) noexcept;
		static uint8_t MODWIN_ABI_CALL ApiInputKeyPressed(
			void* context, uint32_t keyCode) noexcept;
		static uint8_t MODWIN_ABI_CALL ApiInputKeyReleased(
			void* context, uint32_t keyCode) noexcept;
		static ModwinResult MODWIN_ABI_CALL ApiLoadTexture(
			void* context, const char* textureId, const char* resourceDirectory) noexcept;
		static ModwinEntityHandle MODWIN_ABI_CALL ApiCreateEntity(
			void* context, const ModwinEntityCreateInfo* createInfo) noexcept;
		static ModwinResult MODWIN_ABI_CALL ApiDestroyEntity(
			void* context, ModwinEntityHandle entity) noexcept;
		static ModwinResult MODWIN_ABI_CALL ApiGetEntityPosition(
			void* context, ModwinEntityHandle entity, ModwinVec2* position) noexcept;
		static ModwinResult MODWIN_ABI_CALL ApiSubmitEntityCommands(
			void* context,
			const ModwinEntityCommand* commands,
			uint32_t commandCount) noexcept;

		ModwinGameApi m_GameApi{};
		ModwinEngineApi m_EngineApi{};
		RuntimeWorld m_RuntimeWorld;
		bool m_GameInitialized = false;
		ProjectContext m_ProjectContext;
		std::optional<MapDocument> m_ActiveDocument;
		EditorState m_EditorState;
		EditorMode m_PreviousEditorMode = EditorMode::Edit;
		EditorUI m_EditorUI;
		EditorController m_EditorController;
		MapViewport m_MapViewport;
		FrameClock m_FrameClock;
		float m_FixedStepAccumulator = 0.0F;
		bool m_CloseConfirmationRequested = false;
		SDL_Renderer* m_Renderer = nullptr;
		SDL_Window* m_Window = nullptr;
		bool m_IsRunning = false;
	};
}
