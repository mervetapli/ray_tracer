#include <rt/integrators/recraytrace.h>
#include <rt/intersection.h>
#include <rt/solids/solid.h>
#include <rt/materials/dummy.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <core/color.h>
#include <rt/coordmappers/coordmapper.h>

using namespace rt;

//getRadiance
RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const
{
	return this->getRadiance(ray, 0);
}

//getRadiance
RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray, int rec_count) const
{
	Intersection intersection = world->scene->intersect(ray);

	if (!intersection)
	{
		return RGBColor(0.0f, 0.0f, 0.0f);
	}

	Vector normal = dot(ray.d_, intersection.normal()) > 0 ? -intersection.normal() : intersection.normal();
	RGBColor color = intersection.solid_->material_->getEmission(intersection.solid_->tex_mapper_->getCoords(intersection), normal, -ray.d_);

	Material::Sampling sampling = intersection.solid_->material_->useSampling();
	if (sampling != Material::SAMPLING_ALL)
	{
		int size = world->light.size();

		//1-iterate over all light sources.
		for (int i = 0; i < size; ++i)
		{
			LightHit light_hit = world->light[i]->getLightHit(intersection.hitPoint());

			//2-compute shadow ray.
			Ray shadow_ray(intersection.hitPoint() + light_hit.direction*0.00001f, light_hit.direction);

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
	}

	if (rec_count > 10)
	{
		return color;
	}

	if (sampling != Material::SAMPLING_NOT_NEEDED)
	{
		//hybrid or sample case
		Vector normal = dot(ray.d_, intersection.normal()) > 0 ? -intersection.normal() : intersection.normal();
		Material::SampleReflectance sample_reflectance =
			intersection.solid_->material_->getSampleReflectance(intersection.local(), normal, -intersection.ray_.d_);

		//construct secondary ray.
		Ray secondary_ray = Ray(intersection.hitPoint() + sample_reflectance.direction * 0.00001f, sample_reflectance.direction);
		color = color + sample_reflectance.reflectance * this->getRadiance(secondary_ray, rec_count + 1);
	}

	return color;
}