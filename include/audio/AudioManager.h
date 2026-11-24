#ifndef MODWIN_ENGINE_AUDIOMANAGER_H
#define MODWIN_ENGINE_AUDIOMANAGER_H
#include <string>
#include <SDL3/SDL_audio.h>

namespace Modwin
{
	class AudioManager
	{
		public:
			AudioManager();
			~AudioManager();

			bool Init();
			void Update();
			void ShutDown();

			void PlayMusic(std::string path);
			void StopMusic();
			void PlaySound(std::string path);

		private:
			SDL_AudioStream* m_MusicStream;
			SDL_AudioStream* m_SoundStream;
	};
}

#endif //MODWIN_ENGINE_AUDIOMANAGER_H
