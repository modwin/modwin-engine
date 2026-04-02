//
// Created by komvu on 2024-12-06.
//
//#define TINYOBJLOADER_IMPLEMENTATION

#include "Engine.h"

#ifdef ENGINE_H

#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <vector>
//#include <tiny_obj_loader.h>


namespace Winther
{
//	Engine* Engine::GetInstance()
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
	* Initializes the game engine...
	*/
	bool Engine::Init()
	{

		RenderStaticEntities("StaticEntities.json");
		Log::Init();
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			Winther::Log::GetCoreLogger()->info("SDL_Init error...");
			SDL_Log("SDL_Log = SDL_Init error. /n Error: {0}", SDL_GetError());
			return false;
		}
		// Logs result of Init()
		Winther::Log::GetCoreLogger()->info("SDL Initialized correctly.");

		m_Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, NULL);

		if (!m_Device)
		{
			Log::GetCoreLogger()->info("Failed to initialize GPU device. /n Error: {0}", SDL_GetError());
		}
		else
		{
			Log::GetCoreLogger()->info("GPU device was successfully initialized./nGPU device drivers = {0}",
					SDL_GetGPUDeviceDriver(m_Device));
		}


		int w = 1280;
		int h = 720;

		const SDL_DisplayMode* display = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
		if (display == nullptr)
		{
			Log::GetCoreLogger()->info(
					"Failed to retrieve primary display resolution. Setting window resolution to 720p. /n Error: {0}",
					SDL_GetError());
		}
		else
		{
			w = display->w;
			h = display->h;
		}

		m_Window = SDL_CreateWindow("Winther engine", 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
		if (!m_Window)
		{
			Log::GetCoreLogger()->info("Failed to initialize window. /n Error: {0}", SDL_GetError());
			return false;
		}
		m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
//		SDL_CreateWindowAndRenderer("Winther Engine", w, h, 0, &m_Window, &m_Renderer)
		if (!m_Renderer)
		{
			Log::GetCoreLogger()->info("Failed to initialize renderer. /n Error: {0}", SDL_GetError());
			return false;
		}


		if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window))
		{
			Log::GetCoreLogger()->info("Failed to claim window for GPU device. {}", SDL_GetError());
		}

//		SDL_SetWindowSurfaceVSync(m_Window, SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE);

		return m_IsRunning = true;

	}

	// Main game loop.
	void Engine::Run()
	{
//		SDL_GLContext gl = SDL_GL_CreateContext(m_Window);
//		SDL_GL_SetSwapInterval(1);
//
//		glewExperimental = true;
//		glewInit();
//
//		tinyobj::attrib_t attr;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//		std::string warn, err;
//		tinyobj::ObjReader reader;
//		tinyobj::ObjReaderConfig reader_config;
//		reader_config.mtl_search_path = "./";


//		ParseFromFile("Residential_Buildings_001.obj", reader_config);
//		if(!reader.Warning().empty())
//			std::cerr << reader.Warning() << std::endl;
//
//		if(!reader.Error().empty())
//			std::cerr << reader.Error() << std::endl;
//
//
//		if(!ret)
//			exit(1);
		while (m_IsRunning)
		{
			Render();
			EventListener();
			m_EntityManager.Update();
//			SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0x00, 0x00, 0x10);
		}
	}

	/*
	 *  Quits safely
	 */

	void Engine::Quit()
	{
		m_IsRunning = false;
		TextureManager::GetInstance()->Clean();
		Engine::~Engine();
	}

	/*
	 *  Listens to events
	 */

	void Engine::EventListener()
	{
		int counter = 0;
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				Quit();
			}
//			if (event.type == SDL_EVENT_MOUSE_MOTION)
//			{
//				SDL_SetRenderDrawColorFloat(m_Renderer, event.motion.x / 1000, event.motion.y / 1000,
//						event.motion.y / 1000, event.motion.x / 1000);
//			}
//			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
//			{

//				SDL_RenderClear(m_Renderer);
//				SDL_Surface* surface = SDL_GetWindowSurface(m_Window);
//				surface = IMG_Load("C:/Users/komvu/CLionProject/winther-engine/resources/textures/building03_c.tga");
//
//
//				if(surface == nullptr)
//					SDL_Log("Error: %s", SDL_GetError());
//
////				auto texture = SDL_CreateTextureFromSurface(m_Renderer, surface);

//				if(SDL_SetRenderTarget(m_Renderer,texture))
//					SDL_Log("Error: %s", SDL_GetError());


//				if(!SDL_RenderTexture(m_Renderer, texture, nullptr, nullptr))
//					SDL_Log("Error: %s", SDL_GetError());

//				if (!SDL_UpdateWindowSurface(m_Window))
//				{
//					SDL_Log("Error: %s", SDL_GetError());
//				}
//
//				SDL_SetRenderTarget(m_Renderer, nullptr);
//				SDL_SetWindowSurfaceVSync(m_Window, 0);
//				SDL_RenderPresent(m_Renderer);
//				SDL_Delay(1000);
//
//

//				auto texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ABGR64, SDL_TEXTUREACCESS_STATIC, 512,
//						5120);
//				auto gpuTexture = SDL_CreateGPUTexture(m_Device, NULL);
//				SDL_RenderTexture(m_Renderer, texture, NULL, NULL);


//			}
//		}
//		counter++;
//		return;
//		if (counter % 100 == 0)
//		{
//			Log::GetCoreLogger()->info("Handling events...");
		}
	}

	/*
	 *  Reads incoming data...
	 */

	void Engine::Read()
	{
		Log::GetCoreLogger()->info("READING...");
	}

	/*
	 * Renders frames
	 */

	void Engine::Render()
	{
		SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
		SDL_RenderClear(m_Renderer);

		std::string path = "C:/Users/komvu/CLionProject/winther-engine/resources/obj/textures/";
		TextureManager::GetInstance()->Load("box", path + "Box_D.jpg");
		TextureManager::GetInstance()->Draw("box", 100, 50, 1024, 1024);
		SDL_RenderPresent(m_Renderer);

	}


	/*
	 * Client game loop.
	 */

//	Client::Client() = default;


//	Engine* Engine::CreateClient()
//	{
//		if (s_INSTANCE == nullptr)
//		{
//			s_INSTANCE = new Engine();
//		}
//
//		return s_INSTANCE;
//	}

	void Engine::HandleInputs()
	{

	}

	void Engine::LoadEntities()
	{


	}

	bool Engine::RenderStaticEntities(const std::string&& filename)
	{
		for (const auto& e: m_EntityManager.GetStaticEntities())
		{

		}
		return false;
	}


	Engine::~Engine()
	{
		SDL_DestroyRenderer(GetInstance()->m_Renderer);
		SDL_DestroyWindow(GetInstance()->m_Window);
		GetInstance()->m_Device = nullptr;
		GetInstance()->m_Data = nullptr;
		GetInstance()->m_DisplayMode = nullptr;
	}

	Engine* Engine::GetInstance()

	{
		if (s_INSTANCE == nullptr)
		{
			s_INSTANCE = new Engine();
		}

		return s_INSTANCE;
	}




//	Client* Client::GetInstance()
//	{
//		if (s_INSTANCE == nullptr)
//		{
//			return new Client();
//		}
//
//		return s_INSTANCE;
//
//	}
};


#endif
