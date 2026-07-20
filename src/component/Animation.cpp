//
// Created by komvu on 2024-12-27.
//

#include "component/Animation.h"

#include <utility>

namespace Modwin
{
	void Animation::SetValues(std::string id, int spriteRow, int frameCount, int animationSpeed, SDL_FlipMode flipMode)
	{
		m_TextureID = std::move(id);
		m_SpriteRow = spriteRow;
		m_FrameCount = frameCount;
		m_FlipMode = flipMode;
		m_AnimationSpeed = animationSpeed;
	}

	void Animation::Draw(float x, float y, float spriteW, float spriteH) const
	{
		TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spriteW, spriteH, m_SpriteRow, m_SpriteFrame, m_FlipMode);
//		TextureManager::GetInstance()->Draw(m_TextureID,x, y, spriteW, spriteH, m_FlipMode);
	}

	void Animation::Update()
	{
		m_SpriteFrame = (SDL_GetTicks() / m_AnimationSpeed) % m_FrameCount;

	}
} // Modwin