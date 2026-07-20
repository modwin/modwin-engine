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
#include <GL/glu.h>
#include "graphics/TextureManager.h"
#include <physics/Transform.h>
#include "character/Player.h"
#include <imgui.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_timer.h>
#include <physics/RigidBody.h>
#include <character/Character.h>
#include <entity/Entity.h>
#include <input/InputHandler.h>
#include <graphics/MapParser.h>
#include <graphics/TileMap.h>


namespace Winther
{


	class Engine
	{
		public:

			inline Engine(const Engine&) = delete;

			inline Engine& operator=(const Engine&) = delete;

			inline static Engine* GetInstance()
			{
				if (s_INSTANCE == nullptr)
				{
					s_INSTANCE = new Engine();
				}

				return s_INSTANCE;
			}

			void Run();

			bool Init();

			bool InitImgui();

			void Quit();

			void Render();

			bool EventListener();

			inline bool IsRunning();

			SDL_Renderer* GetRenderer();

//			bool RenderStaticEntities(const std::string&& filename);

			void LoadResources();

			void Update();

			~Engine();

		private:
			Engine(){};
			static Engine* s_INSTANCE;
			SDL_GPUDevice* m_Device;
			SDL_Renderer* m_Renderer;
			SDL_Window* m_Window;
			TileMap* m_CurrentLevel;
			bool m_IsRunning;
			int* m_Data;



	};


} // Winther


#endif //ENGINE_H
