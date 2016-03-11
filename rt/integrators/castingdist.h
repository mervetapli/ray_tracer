#ifndef CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER
#define CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER

#include <rt/integrators/integrator.h>
#include <core/color.h>

namespace rt
{
	class World;
	class Ray;
	class RGBColor;

	class RayCastingDistIntegrator : public Integrator
	{
	public:
		RayCastingDistIntegrator(World* world, const RGBColor& near_color, float near_distance, const RGBColor& far_color, float far_distance);
		virtual RGBColor getRadiance(const Ray& ray) const;

	private:
		RGBColor near_color_, far_color_;
		float near_distance_, far_distance_;
	};
}

#endif