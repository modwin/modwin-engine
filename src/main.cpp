#include "core/Engine.h"

namespace Modwin
{

	#include <SDL3/SDL_main.h>

	int main(int argc, char** argv)
	{

		Engine::GetInstance()->Init();
		Engine::GetInstance()->Run();
		Engine::GetInstance()->Quit();

		SDL_Quit();
		return 0;
	};
}


