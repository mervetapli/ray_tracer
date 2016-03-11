#ifndef CG1RAYTRACER_MATERIALS_MIRROR_HEADER
#define CG1RAYTRACER_MATERIALS_MIRROR_HEADER

#include <rt/materials/material.h>

namespace rt
{
	class MirrorMaterial : public Material
	{
	public:
		MirrorMaterial(float eta, float kappa);
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual Sampling useSampling() const;

	private:
		float eta_;
		float kappa_;
	};
}

#endif