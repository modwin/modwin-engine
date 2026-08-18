#ifndef MODWIN_ENGINE_TILECOORDINATE_H
#define MODWIN_ENGINE_TILECOORDINATE_H

#include <optional>

namespace Modwin
{
	struct TileCoordinate
	{
		int column = -1;
		int row = -1;

		[[nodiscard]] bool operator==(const TileCoordinate& other) const noexcept
		{
			return column == other.column && row == other.row;
		}
	};

	struct MapViewport
	{
		float x = 0.0F;
		float y = 0.0F;
		float width = 0.0F;
		float height = 0.0F;

		[[nodiscard]] bool Contains(float screenX, float screenY) const noexcept;
	};

	[[nodiscard]] std::optional<TileCoordinate> ScreenToTile(
		float screenX,
		float screenY,
		const MapViewport& viewport,
		float cameraX,
		float cameraY,
		float zoom,
		int tileWidth,
		int tileHeight,
		int mapWidth,
		int mapHeight) noexcept;
}

#endif // MODWIN_ENGINE_TILECOORDINATE_H
