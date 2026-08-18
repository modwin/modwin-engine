#include "core/Engine.h"
#include "core/ResourcePaths.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <utility>

int main(int argc, char** argv)
{
	Modwin::ProjectContext projectContext = argc > 1
		? Modwin::CreateProjectContext(argv[1])
		: Modwin::CreateDefaultProjectContext();

	auto* engine = Modwin::Engine::GetInstance();
	if (!engine->Init(std::move(projectContext)))
	{
		SDL_Quit();
		return 1;
	}

	engine->Run();
	engine->Quit();
	SDL_Quit();

	return 0;
}
