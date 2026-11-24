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
			AudioManager() = default;
			~AudioManager() = default;;

			bool Init();
			void Update();
			void Shutdown();

			bool PlayMusic(const std::string& path, bool loop = true);
			void SetMusicVolume(float volume);

			void PlaySfx(const std::string& soundId, float volume = 1.0);
			void SetSfxVolume(float volume);
			bool LoadSfx(const std::string& soundId, const std::filesystem::path& path);

		private:
			SDL_AudioDeviceID m_DeviceId = 0;
			static constexpr std::size_t MaxSfxChannels = 16;

			bool m_LoopMusic = true;
			float m_MusicVolume = 1;
			SDL_AudioStream* m_MusicStream = nullptr;
			std::string m_MusicPath;
			std::unique_ptr<SoundClip> m_MusicTrack;

			float m_SfxVolume = 1;
			std::array<SDL_AudioStream*, MaxSfxChannels> m_SfxChannels{};
			std::size_t m_NextChannelIndex = 0; // Next channel index that is free

			std::unordered_map<std::string, SoundClip> m_SoundClips{};
	};
}

#endif //MODWIN_ENGINE_AUDIOMANAGER_H
