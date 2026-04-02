//
// Created by komvu on 2024-12-17.
//

#ifndef WINTHER_ENGINE_TEXTUREMANAGER_H
#define WINTHER_ENGINE_TEXTUREMANAGER_H

#include <string>
#include <SDL3/SDL.h>

namespace Winther
{

	class TextureManager
	{
	private:


	public:

		TextureManager() = default;

		bool Load(std::string id, std::string filename);

		void Remove(std::string id);

		void Clean();

		void Draw(std::string id, int x, int y, int w, int h, SDL_FlipMode flipMode = SDL_FLIP_NONE);

		~TextureManager() = default;

	};

} // Winther

#endif //WINTHER_ENGINE_TEXTUREMANAGER_H
