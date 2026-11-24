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
	 *	Saves music track with @path if it isn't already saved to @m_MusicTrack and plays it.
	 *	@param path path of music track to be played.
	 *	@param volume
	 */
	bool AudioManager::PlayMusic(const std::string& path, float volume,bool loop)
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

	void AudioManager::SetMusicVolume(float volume)
	{
	}

	/**
	 * Plays a SoundClip with the ID of @param soundId with volume @param volume
	 * @param soundId plays sound effects clip with the ID of @soundId
	 * @param volume float value 0.0 to 1.0 interpreted as a percentage.
	 */
	void AudioManager::PlaySfx(const std::string& soundId, float volume)
	{
		auto iter = m_SoundClips.find(soundId);
		if (iter != m_SoundClips.end()) return;

		const SoundClip& clip = iter->second;

		SDL_AudioStream* allocStream = nullptr;
		for (SDL_AudioStream* stream : m_SfxChannels)
		{
			// Iterates over streams until it finds an available one.
			if (SDL_GetAudioStreamAvailable(stream))
			{
				allocStream = stream;
				break;
			}
		}
		// Forces allocation of a stream to the SoundClip if all are busy.
		if (allocStream == nullptr)
		{
			allocStream = m_SfxChannels[m_NextChannelIndex];
			m_NextChannelIndex = (m_NextChannelIndex + 1) % MaxSfxChannels;
			SDL_ClearAudioStream(allocStream);
		}

		SDL_SetAudioStreamGain(allocStream, volume);
		SDL_SetAudioStreamFormat(allocStream, &clip.GetSpec(), nullptr);
		SDL_PutAudioStreamData(allocStream, clip.GetBuffer(), clip.GetLength());	}

	void AudioManager::SetSfxVolume(float volume)
	{
	}
}
