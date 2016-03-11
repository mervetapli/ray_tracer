#ifndef CG1RAYTRACER_RENDERER_HEADER
#define CG1RAYTRACER_RENDERER_HEADER

#include <core/scalar.h>

namespace rt
{
	class Image;
	class Camera;
	class Integrator;

	class Renderer
	{
	public:
		Renderer(Camera* camera, Integrator* integrator);
		void setSamples(uint samples);
		void render(Image& img);
		void test_render1(Image& img);
		void test_render2(Image& img);

	private:
		Camera* camera_;
		Integrator* integrator_;
		uint samples_;
	};
}

#endif