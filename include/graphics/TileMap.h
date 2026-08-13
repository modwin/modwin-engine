#ifndef MODWIN_ENGINE_TILEMAP_H
#define MODWIN_ENGINE_TILEMAP_H

#include <graphics/SurfaceLayer.h>
#include <vector>

namespace Modwin
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

#endif //MODWIN_ENGINE_TILEMAP_H
