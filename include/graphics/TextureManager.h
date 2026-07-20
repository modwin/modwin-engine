//
// Created by komvu on 2024-12-17.
//


#pragma once

#ifndef WINTHER_ENGINE_TEXTUREMANAGER_H
#define WINTHER_ENGINE_TEXTUREMANAGER_H

#include <string>
#include <SDL3/SDL.h>
#include <map>
#include <string>
#include "tinyxml2.h"

namespace Winther
{

	class TextureManager
	{

	public:

		static TextureManager* GetInstance();

		bool Load(std::string&& name, std::string filePath);

		void Remove(std::string id);

		void Clean();

		void Draw(const std::string& textureID, float x, float y, float w, float h, SDL_FlipMode flipMode = SDL_FLIP_NONE);

		void DrawFrame(const std::string& textureID, float x, float y, float w, float h, int spriteRow, int frame, SDL_FlipMode flip);

		void DrawTile(std::string id, int tileWidth, int x, int y, int row, int frame, SDL_FlipMode flipMode = SDL_FLIP_NONE);

		void Destroy(std::string id);

		~TextureManager() = default;


	private:
		TextureManager(){};
		std::map<std::string, SDL_Texture*> m_Textures;
		static TextureManager* s_INSTANCE;
	};

} // Winther

#endif //WINTHER_ENGINE_TEXTUREMANAGER_H
