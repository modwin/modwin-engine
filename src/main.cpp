#include "core/Engine.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto* engine = Modwin::Engine::GetInstance();
	if (!engine->Init())
	{
		SDL_Quit();
		return 1;
	}

	engine->Run();
	engine->Quit();
	SDL_Quit();

	return 0;
}
