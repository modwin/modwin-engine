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

	void AudioManager::Shutdown()
	{
		if (m_MusicStream)
		{
			SDL_DestroyAudioStream(m_MusicStream);
			m_MusicStream = nullptr;
		}

		for (SDL_AudioStream*& stream: m_SfxChannels)
		{
			if (stream)
			{
				SDL_DestroyAudioStream(stream);
				stream = nullptr;
			}
		}

		m_SoundClips.clear();

		if (m_DeviceId != 0)
		{
			SDL_CloseAudioDevice(m_DeviceId);
			m_DeviceId = 0;
		}
	}

	void AudioManager::Update()
	{
		// Update music

		// Update sound effects clips

	}

	/**
	 *
	 * @param path saves music track with @path if it isn't already saved to @m_MusicTrack and plays it.
	 */
	bool AudioManager::PlayMusic(std::string path, bool loop)
	{
		if (m_MusicPath == path)
		{
			return true;
		}

		m_MusicPath = path;
		m_MusicTrack = std::make_unique<SoundClip>(path);

		if (!m_MusicTrack->IsValid())
		{
			m_MusicTrack.reset();
			return false;
		}

		SDL_ClearAudioStream(m_MusicStream);
		SDL_SetAudioStreamFormat(m_MusicStream, &m_MusicTrack->GetSpec(), nullptr);
		SDL_PutAudioStreamData(m_MusicStream, m_MusicTrack->GetBuffer(), m_MusicTrack->GetLength());
		return true;
	}


}
