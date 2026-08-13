#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>

namespace Modwin
{
	class TextureManager
	{
	public:
		static TextureManager* GetInstance();

		bool Load(const std::string& name, const std::string& directory);
		void Destroy(const std::string& id);
		void Clean();

		void Draw(const std::string& textureId, float x, float y, float width, float height,
			SDL_FlipMode flipMode = SDL_FLIP_NONE);
		void DrawFrame(const std::string& textureId, float x, float y, float width, float height,
			int spriteRow, int frame, SDL_FlipMode flipMode);
		void DrawTile(const std::string& textureId, int tileWidth, int x, int y, int row, int frame,
			SDL_FlipMode flipMode = SDL_FLIP_NONE);

		~TextureManager() = default;

	private:
		TextureManager() = default;
		[[nodiscard]] SDL_Texture* FindTexture(const std::string& id) const;

		std::unordered_map<std::string, SDL_Texture*> m_Textures;
	};
}
