//
// Created by komvu on 2024-12-06.
//

#ifndef CLIENT_CPP

#define CLIENT_CPP
#include "core/Engine.h"

	#ifdef ENGINE_H
		#ifdef WINTHER_ENGINE_TEXTUREMANAGER_H

namespace Winther
{

	#include <SDL3/SDL_main.h>




	int main(int argc, char** argv)
	{
		auto client = Engine::GetInstance();

		client->Init();
		client->InitImgui();
		client->Run();
		client->Quit();

		SDL_Quit();
		return 0;
	};
}

		#endif
	#endif
#endif // CLIENT_CPP

