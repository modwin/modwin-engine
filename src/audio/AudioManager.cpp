#include "audio/AudioManager.h"

#include "Log.h"
#include <algorithm>

namespace Modwin
{
	bool AudioManager::Init()
	{
		m_DeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (m_DeviceId == 0) return false;

		m_MusicStream = SDL_CreateAudioStream(nullptr, nullptr);
		SDL_BindAudioStream(m_DeviceId, m_MusicStream);

		for (std::size_t i = 0; i < MaxSfxChannels; ++i)
		{
			m_SfxChannels[i] = SDL_CreateAudioStream(nullptr, nullptr);
			SDL_BindAudioStream(m_DeviceId, m_SfxChannels[i]);
		}

		SDL_ResumeAudioDevice(m_DeviceId);
		return true;
	}
}
