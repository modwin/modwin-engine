//
// Created by komvu on 2025-01-04.
//

#ifndef WINTHER_ENGINE_TILEMAP_H
#define WINTHER_ENGINE_TILEMAP_H

#include <graphics/SurfaceLayer.h>
#include <vector>

namespace Winther
{
	class TileMap
	{
	public:
		TileMap(){}

		void Render()
		{
			for(auto layer : m_SurfaceLayers)
			{
				layer->Render();
			}
		}

		void Update()
		{
			for(auto layer : m_SurfaceLayers)
			{
				layer->Update();
			}
		}

		inline std::vector<SurfaceLayer*> GetSurfaceLayers(){return m_SurfaceLayers;}

	private:
		friend class MapParser;
		std::vector<SurfaceLayer*> m_SurfaceLayers;
	};
}

#endif //WINTHER_ENGINE_TILEMAP_H
