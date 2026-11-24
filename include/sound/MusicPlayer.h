#ifndef MODWIN_ENGINE_MUSICPLAYER_H
#define MODWIN_ENGINE_MUSICPLAYER_H
#include <SDL3/SDL_audio.h>

namespace Modwin
{
	struct WAV
	{
		SDL_AudioSpec spec;
		char* path;
		Uint8* data;
		Uint32 dataLength;
	};

	class MusicPlayer
	{
		public:
			void LoadMusic();

			void Init();

		private:
			SDL_AudioStream* m_AudioStream = nullptr;
			WAV* m_WAV = nullptr;
	};
}
#endif //MODWIN_ENGINE_MUSICPLAYER_H
