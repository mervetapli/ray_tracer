#ifndef CG1RAYTRACER_MATERIALS_COMBINE_HEADER
#define CG1RAYTRACER_MATERIALS_COMBINE_HEADER

#include <rt/materials/material.h>
#include <rt/materials/combine.h>
#include <vector>
#include <utility>

namespace rt
{
	class CombineMaterial : public Material
	{
	public:
		CombineMaterial();
		void add(Material* material, float weight);
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const;
		virtual Sampling useSampling() const;

	private:
		std::vector<std::pair<Material*, float>> material_list_;
		Sampling sampling_;
	};
}

#endif