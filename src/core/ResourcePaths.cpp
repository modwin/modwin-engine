#include "core/ResourcePaths.h"

#include <SDL3/SDL_filesystem.h>

namespace Modwin
{
	std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath)
	{
		const char* basePath = SDL_GetBasePath();
		if (basePath == nullptr)
		{
			return std::filesystem::path("resources") / relativePath;
		}

		return std::filesystem::path(basePath) / "resources" / relativePath;
	}
}
