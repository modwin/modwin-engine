#include "graphics/TextureManager.h"
#include "Log.h"
#include "core/Engine.h"
#include "core/ResourcePaths.h"
#include <SDL3_image/SDL_image.h>

namespace Modwin
{
	TextureManager* TextureManager::GetInstance()
	{
		static TextureManager instance;
		return &instance;
	}

	bool TextureManager::Load(const std::string& name, const std::string& directory)
	{
		const auto imagePath = GetResourcePath(directory) / (name + ".png");
		return LoadFromFile(name, imagePath);
	}

	bool TextureManager::LoadFromFile(
		const std::string& textureId, const std::filesystem::path& path)
	{
		if (textureId.empty() || path.empty())
		{
			Log::GetCoreLogger()->error("A texture ID and file path are required.");
			return false;
		}

		if (m_Textures.find(textureId) != m_Textures.end())
		{
			return true;
		}

		SDL_Surface* surface = IMG_Load(path.string().c_str());
		if (surface == nullptr)
		{
			Log::GetCoreLogger()->error("Failed to load texture '{}': {}", path.string(), SDL_GetError());
			return false;
		}

		SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();
		if (renderer == nullptr)
		{
			SDL_DestroySurface(surface);
			Log::GetCoreLogger()->error("Cannot create texture '{}' without an SDL renderer.", textureId);
			return false;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);

		if (texture == nullptr)
		{
			Log::GetCoreLogger()->error("Failed to create texture '{}': {}", textureId, SDL_GetError());
			return false;
		}

		m_Textures.emplace(textureId, texture);
		return true;
	}

	void TextureManager::Destroy(const std::string& id)
	{
		const auto texture = m_Textures.find(id);
		if (texture == m_Textures.end())
		{
			return;
		}

		SDL_DestroyTexture(texture->second);
		m_Textures.erase(texture);
	}

	void TextureManager::Clean()
	{
		for (const auto& [id, texture] : m_Textures)
		{
			(void)id;
			SDL_DestroyTexture(texture);
		}
		m_Textures.clear();
	}

	SDL_Texture* TextureManager::GetTexture(const std::string& id) const noexcept
	{
		const auto texture = m_Textures.find(id);
		return texture != m_Textures.end() ? texture->second : nullptr;
	}

	SDL_Texture* TextureManager::FindTexture(const std::string& id) const
	{
		SDL_Texture* texture = GetTexture(id);
		if (texture == nullptr)
		{
			SDL_Log("Texture ID '%s' was not found.", id.c_str());
			return nullptr;
		}

		return texture;
	}

	void TextureManager::Draw(const std::string& textureId, float x, float y, float width, float height,
		SDL_FlipMode flipMode)
	{
		SDL_Texture* texture = FindTexture(textureId);
		if (texture == nullptr)
		{
			return;
		}

		const SDL_FRect source = {0, 0, width, height};
		const SDL_FRect destination = {x, y, width, height};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), texture, &source, &destination, 0, nullptr, flipMode);
	}

	void TextureManager::DrawFrame(const std::string& textureId, float x, float y, float width, float height,
		int spriteRow, int frame, SDL_FlipMode flipMode)
	{
		SDL_Texture* texture = FindTexture(textureId);
		if (texture == nullptr)
		{
			return;
		}

		const SDL_FRect source = {width * frame, height * spriteRow, width, height};
		const SDL_FRect destination = {x, y, width, height};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), texture, &source, &destination, 0, nullptr, flipMode);
	}

	void TextureManager::DrawTile(const std::string& textureId, int tileWidth, float x, float y, int row, int frame,
		SDL_FlipMode flipMode)
	{
		SDL_Texture* texture = FindTexture(textureId);
		if (texture == nullptr)
		{
			return;
		}

		const auto size = static_cast<float>(tileWidth);
		const SDL_FRect source = {size * frame, size * row, size, size};
		const SDL_FRect destination = {x, y, size, size};
		SDL_RenderTextureRotated(Engine::GetInstance()->GetRenderer(), texture, &source, &destination, 0, nullptr, flipMode);
	}
}
