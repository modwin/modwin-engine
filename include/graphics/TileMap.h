#ifndef MODWIN_ENGINE_TILEMAP_H
#define MODWIN_ENGINE_TILEMAP_H

#include "graphics/TileLayer.h"

#include <utility>
#include <vector>

namespace Modwin
{
	class TileMap
	{
	public:
		TileMap() = default;
		TileMap(int width, int height, int tileWidth, int tileHeight) noexcept
			: m_Width(width),
			  m_Height(height),
			  m_TileWidth(tileWidth),
			  m_TileHeight(tileHeight)
		{
		}

		void Render(const float cameraX = 0.0F, const float cameraY = 0.0F) const
		{
			for (const auto& layer : m_Layers)
			{
				layer.Render(m_Tilesets, cameraX, cameraY);
			}
		}

		void Update()
		{
			for (auto& layer : m_Layers)
			{
				layer.Update();
			}
		}

		void AddTileset(Tileset tileset)
		{
			m_Tilesets.push_back(std::move(tileset));
		}

		void AddLayer(TileLayer layer)
		{
			m_Layers.push_back(std::move(layer));
		}

		[[nodiscard]] const TilesetVec& GetTilesets() const noexcept
		{
			return m_Tilesets;
		}

		[[nodiscard]] std::vector<TileLayer>& GetLayers() noexcept
		{
			return m_Layers;
		}

		[[nodiscard]] const std::vector<TileLayer>& GetLayers() const noexcept
		{
			return m_Layers;
		}

		[[nodiscard]] int GetWidth() const noexcept { return m_Width; }
		[[nodiscard]] int GetHeight() const noexcept { return m_Height; }
		[[nodiscard]] int GetTileWidth() const noexcept { return m_TileWidth; }
		[[nodiscard]] int GetTileHeight() const noexcept { return m_TileHeight; }

	private:
		int m_Width = 0;
		int m_Height = 0;
		int m_TileWidth = 0;
		int m_TileHeight = 0;
		TilesetVec m_Tilesets;
		std::vector<TileLayer> m_Layers;
	};
}

#endif // MODWIN_ENGINE_TILEMAP_H
