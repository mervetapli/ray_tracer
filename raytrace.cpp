#include <rt/integrators/raytrace.h>
#include <rt/intersection.h>
#include <rt/solids/solid.h>
#include <rt/materials/dummy.h>
#include <rt/world.h>
#include <rt/lights/pointlight.h>
#include <core/color.h>
#include <rt/coordmappers/coordmapper.h>

using namespace rt;

//getRadiance
RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const
{
	Intersection intersection = world->scene->intersect(ray);

	if (!intersection)
	{
		return RGBColor(0.0f, 0.0f, 0.0f);
	}

	Vector normal = intersection.normal();

	if (dot(ray.d_, intersection.normal()) >= dot(ray.d_, -intersection.normal()) && dot(ray.d_, intersection.normal()) > 0)
	{
		normal = -intersection.normal();
	}

	int size = world->light.size();

	RGBColor color = intersection.solid_->material_->getEmission(intersection.solid_->tex_mapper_->getCoords(intersection), normal, -ray.d_);
	//1-iterate over all light sources.
	for (int i = 0; i < size; ++i)
	{
		LightHit light_hit = world->light[i]->getLightHit(intersection.hitPoint());

		//2-compute shadow ray.
		Ray shadow_ray(intersection.hitPoint() + light_hit.direction*0.00001, light_hit.direction);

		//3-confirm that the shadow ray and the primary ray leave the surface on the same side.
		if (dot(shadow_ray.d_, normal) <= 0)
		{
			continue;
		}

		//4-trace the shadow ray
		Intersection obstacle = world->scene->intersect(shadow_ray, light_hit.distance);

		//5-if no obstacles found, query the light for intensity.
		if (!obstacle.b_intersection_)
		{
			const Material& material = *intersection.solid_->material_;
			const Light& light = *world->light[i];
			//6-compute the amount of reflected light.
			color = color + light.getIntensity(light_hit)*material.getReflectance(intersection.solid_->tex_mapper_->getCoords(intersection)
				, normal, -ray.d_, light_hit.direction);
		}
	}

	return color;
}