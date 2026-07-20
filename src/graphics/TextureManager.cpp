//
// Created by komvu on 2024-12-17.
//

#include "graphics/TextureManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "core/Engine.h"


namespace Winther
{

	TextureManager* TextureManager::s_INSTANCE = nullptr;

	TextureManager* TextureManager::GetInstance()
	{
		if (s_INSTANCE == nullptr)
		{
			s_INSTANCE = new TextureManager;
		}

		return s_INSTANCE;
	}

	bool TextureManager::Load(std::string&& name, std::string filePath)
	{

		if(!m_Textures[name]){

		filePath =  "../resources/" +filePath + name + ".png";
		SDL_Surface* surface = IMG_Load(filePath.c_str());

		if (surface == nullptr)
		{
			Log::GetCoreLogger()->info("Error loading file {0} with filepath = {1}.\n SDL_Error = {2}", name, filePath, SDL_GetError()); // Logs failures to load file.
			std::cout << std::endl;
			return false;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
		if (texture == nullptr)
		{
			Log::GetCoreLogger()->info("Error creating texture. SDL error: {1}", SDL_GetError());
			return false;
		}
			m_Textures[name] = texture;
		return true;
		}

		return false;
	}

	void TextureManager::Remove(std::string id)
	{

	}

	void TextureManager::Clean()
	{
		std::map<std::string, SDL_Texture*>::iterator iter;
		for(iter = m_Textures.begin(); iter != m_Textures.end(); iter++){
			SDL_DestroyTexture(iter->second);
		}
		m_Textures.clear();

	}

	void TextureManager::Draw(const std::string& id, float x, float y, float w, float h, SDL_FlipMode flipMode)
	{
		SDL_FRect src = {0, 0, w, h};
		SDL_FRect dest = {x, y, w, h};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), m_Textures[id] ,&src, &dest, 0,nullptr, flipMode);
	}

	void TextureManager::Destroy(std::string id)
	{
		SDL_DestroyTexture(m_Textures[id]);
		m_Textures.erase(id);
	}

	void TextureManager::DrawFrame(const std::string& textureID, float x, float y, float w, float h, int spriteRow, int frame, SDL_FlipMode flipMode)
	{
		SDL_FRect src = {w*frame, h*spriteRow, w, h};
		SDL_FRect dest = {x, y, w, h};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), m_Textures[textureID] ,&src, &dest, 0,nullptr, SDL_FLIP_NONE);


	}

	void TextureManager::DrawTile(const std::string id, int tileWidth, int x, int y, int row, int frame, SDL_FlipMode flipMode)
	{
		if (m_Textures.find(id) == m_Textures.end()) {
			SDL_Log("Texture ID %s not found", id.c_str());
			return;
		}

		SDL_FRect src = {static_cast<float>(tileWidth*frame), static_cast<float>(tileWidth* (row)), static_cast<float>(tileWidth), static_cast<float>(tileWidth)};
		SDL_FRect dest = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(tileWidth), static_cast<float>(tileWidth)};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), m_Textures[id] ,&src, &dest, 0,nullptr, flipMode);

	}




//	TextureManager* TextureManager::GetInstance()
} // Winther