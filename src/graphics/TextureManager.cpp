//
// Created by komvu on 2024-12-17.
//

#include "TextureManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <Engine.h>


namespace Winther
{

	bool TextureManager::Load(std::string id, std::string filename)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if(surface == nullptr){
			Log::GetCoreLogger()->info("Error loading file %s, SDL_Error = %s", filename, SDL_GetError());
			std::cout << std::endl;
			return false;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
		if(texture == nullptr){
			Log::GetCoreLogger()->info("Error creating texture. SDL error: %s", SDL_GetError());
			return false;
		}

		Log::GetCoreLogger()->info("Texture and surface was correctly loaded.");
		
		return true;
	}

	void TextureManager::Remove(std::string id)
	{

	}

	void TextureManager::Clean()
	{

	}

	void TextureManager::Draw(std::string id, int x, int y, int w, int h, SDL_FlipMode flipMode)
	{


	}
} // Winther