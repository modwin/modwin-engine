#pragma once

#include <filesystem>

namespace Modwin
{
	struct ProjectContext
	{
		ProjectContext();
		~ProjectContext();

		std::filesystem::path projectRoot;
		std::filesystem::path activeMapPath;
		std::filesystem::path mapPath;
		std::filesystem::path mapRoot;
	};

	[[nodiscard]] std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath);
}
