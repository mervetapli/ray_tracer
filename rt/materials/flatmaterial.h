#ifndef CG1RAYTRACER_MATERIALS_FLATMATERIAL_HEADER
#define CG1RAYTRACER_MATERIALS_FLATMATERIAL_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt
{
	class FlatMaterial : public Material
	{
	public:
		FlatMaterial(Texture* texture);
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }

	private:
		Texture* texture_;
	};
}

#endif