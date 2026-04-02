//
// Created by komvu on 2024-12-06.
//
#pragma once


#ifndef ENGINE_H
#define ENGINE_H
#define WINTHER_API

#include "Log.h"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <string>
#include "entity/EntityManager.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <graphics/TextureManager.h>


namespace Winther
{


	class Engine
	{
	public:

		static Engine* GetInstance();

//		static Engine* CreateClient();

		void Run();

		bool Init();

		bool InitWindowAndRenderer(int& w, int& h);

		bool InitDevice();

		bool InitSDL();

		bool InitDisplay(int& w, int& h);

		void Quit();

		void Render();

		void EventListener();

		void Read();

		inline bool IsRunning();

		SDL_Renderer* GetRenderer();

		bool RenderStaticEntities(const std::string&& filename);

		void LoadEntities();

		~Engine();

	private:
		Engine(){};
		static Engine* s_INSTANCE;
		SDL_DisplayMode* m_DisplayMode;
		SDL_GPUDevice* m_Device;
		SDL_Renderer* m_Renderer;
		SDL_Window* m_Window;
		EntityManager m_EntityManager;
		bool m_IsRunning;
		int* m_Data;


		void HandleInputs();

	};

//	class Client : public Engine
//	{
//	public:
//		static Client* GetInstance();
//
//
//
//	private:
//		static Client* s_INSTANCE;
//
//		Client();
//
//		~Client() = default;
//
//	};

} // Winther


#endif //ENGINE_H
