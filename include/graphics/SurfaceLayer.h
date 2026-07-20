//
// Created by komvu on 2025-01-04.
//

#ifndef WINTHER_ENGINE_SURFACELAYER_H
#define WINTHER_ENGINE_SURFACELAYER_H

namespace Winther
{

	class SurfaceLayer
	{
	public:
		SurfaceLayer(){};
		~SurfaceLayer(){};

		virtual void Update()=0;
		virtual void Render()=0;

	private:

	};

} // Winther

#endif //WINTHER_ENGINE_SURFACELAYER_H
