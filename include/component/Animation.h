#ifndef MODWIN_ENGINE_ANIMATION_H
#define MODWIN_ENGINE_ANIMATION_H

#include <SDL3/SDL.h>

#include <string>

namespace Modwin
{
	class Animation
	{
	public:
		void Update(float deltaSeconds) noexcept;
		void Draw(float x, float y, float spriteWidth, float spriteHeight) const;

		void SetValues(
			std::string textureId,
			int spriteRow,
			int frameCount,
			int frameDurationMilliseconds,
			SDL_FlipMode flipMode = SDL_FLIP_NONE);

		[[nodiscard]] int GetCurrentFrame() const noexcept { return m_SpriteFrame; }

	private:
		int m_SpriteRow = 0;
		int m_SpriteFrame = 0;
		int m_FrameCount = 1;
		float m_FrameDurationSeconds = 0.1F;
		float m_ElapsedSeconds = 0.0F;
		std::string m_TextureId;
		SDL_FlipMode m_FlipMode = SDL_FLIP_NONE;
	};
}

#endif // MODWIN_ENGINE_ANIMATION_H
