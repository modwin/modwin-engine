#pragma once

#include <filesystem>

namespace Modwin
{
	struct ProjectContext
	{
		// Base paths
		std::filesystem::path projectRoot;
		std::filesystem::path assetRoot;
		// Sub-paths
		std::filesystem::path audioPath;
		std::filesystem::path spritePath;
		std::filesystem::path mapPath;
		// Specific paths
		std::filesystem::path npcPath;
		std::filesystem::path monsterPath;
		std::filesystem::path soundEffectsPath;
		std::filesystem::path musicPath;
		std::filesystem::path activeMapPath;
	};

	[[nodiscard]] std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath);
	[[nodiscard]] ProjectContext CreateDefaultProjectContext();
	[[nodiscard]] ProjectContext CreateProjectContext(std::filesystem::path activeMapPath);
}
