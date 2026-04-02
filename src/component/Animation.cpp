//
// Created by komvu on 2024-12-27.
//

#include "Animation.h"

#include <utility>

namespace Winther
{
	void Animation::SetProps(std::string id, int spriteRow, int frameCount, int amimationSpeed, SDL_FlipMode flipMode)
	{
		m_Id = std::move(id);
		m_SpriteRow = spriteRow;
		m_FrameCount = frameCount;
		m_FlipMode = flipMode;
	}

	void Animation::Draw(float x, float y, float spriteW, float spriteH)
	{
		TextureManager::GetInstance()->Draw(m_Id,x, y, spriteW, spriteH, m_FlipMode);
	}

	void Animation::Update()
	{

	}
} // Winther