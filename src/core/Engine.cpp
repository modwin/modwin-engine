//
// Created by komvu on 2024-12-06.
//

#include "core/Engine.h"


#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <time/Time.h>


const char* SDL_HINT_IME_SHOW_UI;
namespace Modwin
{
	Engine* Engine::s_INSTANCE = nullptr;

	SDL_Renderer* Engine::GetRenderer()
	{
		if (s_INSTANCE == nullptr)
		{
			return nullptr;
		}
		return m_Renderer;
	}

	bool Engine::IsRunning()
	{
		return m_IsRunning;
	}


	/*
	 * Initializes the necessary elements of the game engine.
	 */

	bool Engine::Init()
	{
		Log::Init();

		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			Modwin::Log::GetCoreLogger()->info("SDL_Init error...");
			SDL_Log("SDL_Log = SDL_Init error.\nError: %s", SDL_GetError());
			return false;
		}


		m_Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, nullptr);

		if (!m_Device)
		{
			Log::GetCoreLogger()->info("Failed to initialize GPU device.\nError: {0}", SDL_GetError());
		}
		else
		{
			Log::GetCoreLogger()->info("GPU device was successfully initialized.\nGPU device drivers = {0}",
					SDL_GetGPUDeviceDriver(m_Device));
		}


		int w = 1920;
		int h = 1080;

		const SDL_DisplayMode* display = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
		if (display == nullptr)
		{
			Log::GetCoreLogger()->info(
					"Failed to retrieve primary display resolution. Setting window resolution to 720p. \nError: {0}",
					SDL_GetError());
		}
		else
		{
			w = display->w;
			h = display->h;
		}

		m_Window = SDL_CreateWindow("Modwin engine", 1280, 720,
				SDL_WINDOW_RESIZABLE);
		auto context = SDL_GL_CreateContext(m_Window);

		if (!m_Window)
		{
			Log::GetCoreLogger()->info("Failed to initialize window.\nError: {0}", SDL_GetError());
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
		SDL_SetRenderVSync(m_Renderer, 1);

		if (!m_Renderer)
		{
			Log::GetCoreLogger()->info("Failed to initialize renderer.\nError: {0}", SDL_GetError());
			return false;
		}

		//
		// Creates a swap chain in order for the GPU to make use
		// of frame buffering for graphics stabilization.
		//


		if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window))
		{
			Log::GetCoreLogger()->info("Failed to claim window for GPU device. {}", SDL_GetError());
			return false;
		}

		// Logs result of Init()
		Modwin::Log::GetCoreLogger()->info("SDL Initialized correctly.");

		if (!InitImgui())
		{
			Log::GetCoreLogger()->info("Failed to initialize ImGui.");
			return false;
		}

		LoadResources();

		return m_IsRunning = true;

	}

	bool Engine::InitImgui()
	{
		std::cout << IMGUI_CHECKVERSION() << std::endl;
		bool error = ImGui::CreateContext();

		ImGuiIO& guiIo = ImGui::GetIO();
		guiIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		(void)guiIo;
		SDL_Window* w = SDL_CreatePopupWindow(m_Window, 200, 200, 1920, 1080, SDL_WINDOW_RESIZABLE);
		SDL_ShowWindow;

		error = ImGui_ImplSDL3_InitForSDLRenderer(w, m_Renderer);
		error = ImGui_ImplSDLRenderer3_Init(m_Renderer);


		return error;
	}


	/*
		Main game loop.
	 		Listen for events ->
	 		Render textures based on those events ->
	 		Update the state of entities and animations based on tick rate ->
			refresh tick rate.
	 */
	void Engine::Run()
	{
		while (m_IsRunning)
		{
			m_IsRunning = EventListener();
			if(m_IsRunning)
			{

				Update();
				Render();


				// Calculates the tick rate for the next delta time value for consistent rendering.
//				ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
				Time::GetInstance()->CalculateTickRate();

			}
		}
	}



	// Continuously listens for inputs and events from the user with SDL API.
	bool Engine::EventListener()
	{
		return InputHandler::GetInstance()->EventListener();
	}

	void Engine::Render()
	{

		SDL_RenderClear(m_Renderer);
		if(m_CurrentLevel != nullptr) m_CurrentLevel->Render();
		EntityManager::GetInstance()->Draw();


//		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);

		ImGui::Render();


		SDL_RenderPresent(m_Renderer);

	}

	// Quits safely
	void Engine::Quit()
	{



		// Clean up data in other data bearing classes.
		EntityManager::GetInstance()->Clean();
		TextureManager::GetInstance()->Clean();
		MapParser::GetInstance()->Clean();


		// Destroying the SDL rendering and window contexts.
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);


	}

	Engine::~Engine()
	{
		// Destroying ImGui context.
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui::DestroyContext();


		m_Device = nullptr;
		m_Data = nullptr;
		m_CurrentLevel = nullptr;
		m_Window = nullptr;
		m_Renderer = nullptr;


	}

	void Engine::Update()
	{

		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		// Gets the time since the last rendered frame.
		float dt = Time::GetInstance()->GetDeltaTime();
		// Updates the state of the map.
		if(m_CurrentLevel != nullptr) m_CurrentLevel->Update();
		// Updates the states of every entity in the game based again on the deltatime calcualted by the class Time.
		EntityManager::GetInstance()->Update(dt);



	}

	// All the values specifying which textures and entities to render by constant strings would
	// get replaced by variables if I had time to implement all the systems needed.
	void Engine::LoadResources()
	{
		// Loads the sprite for the player character.
		Properties properties(SDL_FLIP_NONE, 35, 45 , 64, 64, "player", "sprites/");
		TextureManager::GetInstance()->Load("player", properties.m_Source);
		EntityManager::GetInstance()->addEntity("player", properties);

		// Loads the map and logs the result.
		if(!MapParser::GetInstance()->LoadMap("1", "map"))
		{
			Log::GetCoreLogger()->info("Failed to load the map.");
		}

		// Gets the relevant map.
		m_CurrentLevel = MapParser::GetInstance()->GetTileMap("1");

	}


};


