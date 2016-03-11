#include <rt/integrators/casting.h>
#include <rt/world.h>
#include <rt/intersection.h>
#include <core/color.h>

using namespace rt;

//getRadiance
RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const
{
	Intersection temp = world->scene->intersect(ray);

	if (!temp.b_intersection_)
	{
		return RGBColor(0.0f, 0.0f, 0.0f);
	}

	float cosine_theta = std::abs(dot(ray.d_, temp.normal()));

	return RGBColor(cosine_theta, cosine_theta, cosine_theta);
}