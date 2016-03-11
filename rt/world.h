#ifndef CG1RAYTRACER_WORLD_HEADER
#define CG1RAYTRACER_WORLD_HEADER

#include <rt/primitive.h>

namespace rt
{
	class Light;

	class World 
	{
	public:
		typedef std::vector<Light*> LightVector;

	public:
		Primitive* scene;
		LightVector light;
	};
}

#endif