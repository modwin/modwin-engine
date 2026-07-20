//
// Created by komvu on 2024-12-27.
//
#pragma once

#ifndef WINTHER_ENGINE_ANIMATION_H
#define WINTHER_ENGINE_ANIMATION_H
#include <string>
#include <graphics/TextureManager.h>

namespace Winther
{

	class Animation
	{
	public:

		Animation(){};

		void Update();
		void Draw(float x, float y, float spriteW, float spriteH) const;
		void SetValues(std::string id, int spriteRow, int frameCount, int animationSpeed, SDL_FlipMode flipMode = SDL_FLIP_NONE);

		int m_SpriteRow, m_SpriteFrame;
		int	m_AnimationSpeed, m_FrameCount;
		std::string m_TextureID;
		SDL_FlipMode m_FlipMode;

	};

} // Winther

#endif //WINTHER_ENGINE_ANIMATION_H
