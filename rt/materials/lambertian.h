#ifndef CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER
#define CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt
{
	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(Texture* emission, Texture* diffuse);
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual Sampling useSampling() const;

	private:
		Texture* emission_;
		Texture* diffuse_;
	};
}

#endif