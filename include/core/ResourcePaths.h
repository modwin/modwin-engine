#pragma once

#include <filesystem>

namespace Modwin
{
	[[nodiscard]] std::filesystem::path GetResourcePath(const std::filesystem::path& relativePath);
}
