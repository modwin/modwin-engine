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
		void Render() const
		{
			for (const auto& layer : m_Layers)
			{
				layer.Render(m_Tilesets);
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

	private:
		TilesetVec m_Tilesets;
		std::vector<TileLayer> m_Layers;
	};
}

#endif // MODWIN_ENGINE_TILEMAP_H
