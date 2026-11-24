#ifndef MODWIN_ENGINE_SOUNDPLAYER_H
#define MODWIN_ENGINE_SOUNDPLAYER_H
#include <filesystem>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

namespace Modwin
{
	class SoundClip
	{
		public:
			explicit SoundClip(const std::filesystem::path& path)
			{
				if (!SDL_LoadWAV(path.string().c_str(), &m_Spec, &m_Buffer, &m_Length))
				{
					m_Buffer = nullptr;
					m_Length = 0;
				}
			}

			~SoundClip()
			{
				if (m_Buffer != nullptr)
				{
					SDL_free(m_Buffer);
					m_Buffer = nullptr;
				}
			}

			SoundClip(const SoundClip&) = delete;
			SoundClip& operator=(const SoundClip&) = delete;

			SoundClip(SoundClip&& other) noexcept
				: m_Spec(other.m_Spec), m_Buffer(other.m_Buffer), m_Length(other.m_Length)
			{
				other.m_Buffer = nullptr;
				other.m_Length = 0;
			}

			[[nodiscard]] bool IsValid() const noexcept { return m_Buffer != nullptr; }
			[[nodiscard]] const SDL_AudioSpec& GetSpec() const noexcept { return m_Spec; }
			[[nodiscard]] const Uint8* GetBuffer() const noexcept { return m_Buffer; }
			[[nodiscard]] Uint32 GetLength() const noexcept { return m_Length; }

		private:
			SDL_AudioSpec m_Spec{};
			Uint8* m_Buffer = nullptr;
			Uint32 m_Length = 0;
	};
}
#endif //MODWIN_ENGINE_SOUNDPLAYER_H
