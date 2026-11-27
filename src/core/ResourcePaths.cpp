#include "core/ResourcePaths.h"
#include <SDL3/SDL_filesystem.h>

#include <system_error>
#include <utility>

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

	ProjectContext CreateDefaultProjectContext()
	{
		ProjectContext context;
		context.assetRoot = GetResourcePath({}).lexically_normal();
		context.projectRoot = context.assetRoot.parent_path();

		context.mapPath = context.assetRoot / "map";
		context.spritePath = context.assetRoot / "sprite";
		context.npcPath = context.spritePath / "npc";

		context.audioPath = context.assetRoot / "audio";
		context.musicPath = context.audioPath / "tracks" / "forest_ambient.wav";
		context.soundEffectsPath = context.audioPath / "effects";

		context.monsterPath = context.spritePath / "monster";
		context.activeMapPath = context.mapPath / "map.tmx";
		return context;
	}

	ProjectContext CreateProjectContext(std::filesystem::path activeMapPath)
	{
		std::error_code error;
		auto normalizedMapPath = std::filesystem::absolute(activeMapPath, error);
		if (error)
		{
			normalizedMapPath = std::move(activeMapPath);
		}

		ProjectContext context;
		context.activeMapPath = normalizedMapPath.lexically_normal();
		context.assetRoot = context.activeMapPath.parent_path().parent_path();
		context.projectRoot =
				context.assetRoot.filename() == "resources"
					? context.assetRoot.parent_path()
					: context.assetRoot;
		return context;
	}
}
