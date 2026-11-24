#ifndef MODWIN_ENGINE_AUDIOMANAGER_H
#define MODWIN_ENGINE_AUDIOMANAGER_H
#include <SDL3/SDL_audio.h>

namespace Modwin
{
	class AudioManager
	{
		public:
			AudioManager();
			~AudioManager();
		private:
			SDL_AudioStream* m_MusicStream;
			SDL_AudioStream* m_SoundStream;
	};
}

#endif //MODWIN_ENGINE_AUDIOMANAGER_H
