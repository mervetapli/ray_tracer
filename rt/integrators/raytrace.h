#ifndef CG1RAYTRACER_INTEGRATORS_RAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RAYTRACING_HEADER

#include <rt/integrators/integrator.h>

namespace rt
{
	class World;
	class Ray;
	class RGBColor;

	class RayTracingIntegrator : public Integrator
	{
	public:
		RayTracingIntegrator(World* p_world) : Integrator(p_world) {}

		virtual RGBColor getRadiance(const Ray& ray) const;
	};
}

#endif