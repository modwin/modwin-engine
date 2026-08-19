#pragma once

#include <filesystem>

namespace Modwin
{
	struct ProjectContext
	{
		std::filesystem::path projectRoot;
		std::filesystem::path assetRoot;
		std::filesystem::path activeMapPath;
	};

	[[nodiscard]] std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath);
	[[nodiscard]] ProjectContext CreateDefaultProjectContext();
	[[nodiscard]] ProjectContext CreateProjectContext(std::filesystem::path activeMapPath);
}
