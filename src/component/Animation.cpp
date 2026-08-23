#include "component/Animation.h"

#include "graphics/TextureManager.h"

#include <algorithm>
#include <utility>

namespace Modwin
{
	void Animation::SetValues(
		std::string textureId,
		const int spriteRow,
		const int frameCount,
		const int frameDurationMilliseconds,
		const SDL_FlipMode flipMode)
	{
		const int validFrameCount = std::max(1, frameCount);
		const float validFrameDuration =
			static_cast<float>(std::max(1, frameDurationMilliseconds)) / 1000.0F;
		const bool changed = m_TextureId != textureId || m_SpriteRow != spriteRow ||
			m_FrameCount != validFrameCount || m_FrameDurationSeconds != validFrameDuration ||
			m_FlipMode != flipMode;

		m_TextureId = std::move(textureId);
		m_SpriteRow = spriteRow;
		m_FrameCount = validFrameCount;
		m_FrameDurationSeconds = validFrameDuration;
		m_FlipMode = flipMode;

		if (changed)
		{
			m_SpriteFrame = 0;
			m_ElapsedSeconds = 0.0F;
		}
	}

	void Animation::Draw(
		const float x,
		const float y,
		const float spriteWidth,
		const float spriteHeight) const
	{
		TextureManager::GetInstance()->DrawFrame(
			m_TextureId,
			x,
			y,
			spriteWidth,
			spriteHeight,
			m_SpriteRow,
			m_SpriteFrame,
			m_FlipMode);
	}

	void Animation::Update(const float deltaSeconds) noexcept
	{
		if (deltaSeconds <= 0.0F || m_FrameCount <= 1)
		{
			return;
		}

		m_ElapsedSeconds += deltaSeconds;
		while (m_ElapsedSeconds >= m_FrameDurationSeconds)
		{
			m_ElapsedSeconds -= m_FrameDurationSeconds;
			m_SpriteFrame = (m_SpriteFrame + 1) % m_FrameCount;
		}
	}
}
