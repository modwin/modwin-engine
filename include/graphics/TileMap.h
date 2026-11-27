#ifndef MODWIN_ENGINE_TILEMAP_H
#define MODWIN_ENGINE_TILEMAP_H

#include "graphics/TileLayer.h"

#include <utility>
#include <vector>

#include "MapObject.h"

namespace Modwin
{
	class TileMap
	{
	public:
		TileMap() = default;
		TileMap(const int width, const int height, const int tileWidth, const int tileHeight) noexcept
			: m_Width(width),
			  m_Height(height),
			  m_TileWidth(tileWidth),
			  m_TileHeight(tileHeight)
		{
		}
		void RenderBackgroundLayers(const float cameraX = 0.0F, const float cameraY = 0.0F) const
		{
			for (int i = 0; i <=1; i++)
			{
				m_Layers[i].Render(m_Tilesets, cameraX, cameraY);
			}
		}

		void RenderForeground(const float cameraX = 0.0F, const float cameraY = 0.0F) const
		{
			for (int i = 2; i == 2; i++)
			{
				m_Layers[i].Render(m_Tilesets, cameraX, cameraY);
			}
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

		void AddObject(MapObject& object)
		{
			m_MapObjects.push_back(std::move(object));
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

		[[nodiscard]] const TileLayer* GetObjectLayer() const noexcept
		{
			for (auto& layer : m_Layers)
			{
				if (layer.IsObjectLayer())
				{
					return &layer;
				}
			}
			return nullptr;
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
		std::vector<MapObject> m_MapObjects;
	};
}

#endif // MODWIN_ENGINE_TILEMAP_H
