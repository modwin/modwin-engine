#ifndef MODWIN_ENGINE_AUDIOMANAGER_H
#define MODWIN_ENGINE_AUDIOMANAGER_H
#include <string>
#include <array>
#include <unordered_map>
#include <SDL3/SDL_audio.h>

#include "SoundClip.h"

namespace Modwin
{
	class AudioManager
	{
		public:
			AudioManager();
			~AudioManager();

			bool Init();
			void Update();
			void Shutdown();

			void PlayMusic(std::string path);
			void StopMusic();
			void SetMusicVolume(float volume);

			void PlaySfx(std::string path);
			void SetSfxVolume(float volume);

		private:
			static constexpr std::size_t MaxSfxChannels = 16;

			SDL_AudioDeviceID m_DeviceId = 0;
			SDL_AudioStream* m_MusicStream;

			std::array<SDL_AudioStream*, MaxSfxChannels> m_SfxChannels{};
			std::size_t m_NextChannelIndex = 0; // Next channel index that is free

			std::unordered_map<std::string, SoundClip> m_SoundClips{};
			std::unique_ptr<SoundClip> m_MusicTrack;
	};
}

#endif //MODWIN_ENGINE_AUDIOMANAGER_H
