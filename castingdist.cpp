#include <rt/integrators/castingdist.h>
#include <rt/world.h>
#include <rt/intersection.h>
#include <core/color.h>

using namespace rt;

RayCastingDistIntegrator::RayCastingDistIntegrator(
	World* world,
	const RGBColor& near_color,
	float near_distance,
	const RGBColor& far_color,
	float far_distance)
	: Integrator(world)
	, near_color_(near_color)
	, near_distance_(near_distance)
	, far_color_(far_color)
	, far_distance_(far_distance)
{}

//getRadiance
RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const
{
	Intersection temp = world->scene->intersect(ray);

	if (!temp.b_intersection_)
	{
		return RGBColor(0.0f, 0.0f, 0.0f);
	}

	float cosine_theta = std::abs(dot(ray.d_, temp.normal()));

	float far = (temp.distance_ - near_distance_) / (far_distance_ - near_distance_);
	if (far > 1)
	{
		far = 1;
	}
	float near = 1 - far;

	return (far_color_*far + near_color_*near)*cosine_theta;
}