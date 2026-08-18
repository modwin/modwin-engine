#include "graphics/TileCoordinate.h"

#include <cmath>

namespace Modwin
{
	bool MapViewport::Contains(const float screenX, const float screenY) const noexcept
	{
		return width > 0.0F && height > 0.0F &&
			screenX >= x && screenX < x + width &&
			screenY >= y && screenY < y + height;
	}

	std::optional<TileCoordinate> ScreenToTile(
		const float screenX,
		const float screenY,
		const MapViewport& viewport,
		const float cameraX,
		const float cameraY,
		const float zoom,
		const int tileWidth,
		const int tileHeight,
		const int mapWidth,
		const int mapHeight) noexcept
	{
		if (!viewport.Contains(screenX, screenY) || zoom <= 0.0F ||
			tileWidth <= 0 || tileHeight <= 0 || mapWidth <= 0 || mapHeight <= 0)
		{
			return std::nullopt;
		}

		const float worldX = (screenX - viewport.x) / zoom + cameraX;
		const float worldY = (screenY - viewport.y) / zoom + cameraY;
		const int column = static_cast<int>(std::floor(worldX / static_cast<float>(tileWidth)));
		const int row = static_cast<int>(std::floor(worldY / static_cast<float>(tileHeight)));

		if (column < 0 || column >= mapWidth || row < 0 || row >= mapHeight)
		{
			return std::nullopt;
		}

		return TileCoordinate{column, row};
	}
}
