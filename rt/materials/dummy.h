#ifndef CG1RAYTRACER_MATERIALS_DUMMY_HEADER
#define CG1RAYTRACER_MATERIALS_DUMMY_HEADER

#include <rt/materials/material.h>

namespace rt
{
	class DummyMaterial : public Material
	{
	public:
		DummyMaterial();
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }

	private:
		RGBColor reflectance_;
	};
}

#endif