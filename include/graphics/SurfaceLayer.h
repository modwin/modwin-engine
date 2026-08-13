#ifndef MODWIN_ENGINE_SURFACELAYER_H
#define MODWIN_ENGINE_SURFACELAYER_H

namespace Modwin
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

} // Modwin

#endif //MODWIN_ENGINE_SURFACELAYER_H
