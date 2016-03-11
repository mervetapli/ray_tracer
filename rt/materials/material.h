#ifndef CG1RAYTRACER_MATERIALS_MATERIAL_HEADER
#define CG1RAYTRACER_MATERIALS_MATERIAL_HEADER

#include <core/vector.h>
#include <core/color.h>

namespace rt
{
	class Material
	{
	public:
		struct SampleReflectance
		{
			Vector direction;
			RGBColor reflectance;

			SampleReflectance(const Vector& p_direction, const RGBColor& p_weight) : direction(p_direction), reflectance(p_weight)
			{}
			SampleReflectance() : direction(0, 0, 1), reflectance(0.0f, 0.0f, 0.0f)
			{}
		};

	public:
		enum Sampling
		{
			SAMPLING_NOT_NEEDED,
			SAMPLING_SECONDARY,
			SAMPLING_ALL
		};

	public:
		/*
		  inDir - the direction the light is incoming from
		  outDir - the direction the light is leaving the surface to the viewer
		  inDir and outDir point -away- from the hit point
		 */
		virtual RGBColor getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const = 0;
		virtual RGBColor getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const = 0;
		virtual SampleReflectance getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const = 0;
		virtual Sampling useSampling() const = 0;
		virtual ~Material() {}
	};
}

#endif