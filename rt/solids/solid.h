#ifndef CG1RAYTRACER_SOLIDS_SOLID_HEADER
#define CG1RAYTRACER_SOLIDS_SOLID_HEADER

#include <rt/primitive.h>
#include <rt/coordmappers/world.h>
#include <core/float4.h>

namespace rt
{
	class Material;
	class CoordMapper;
	/*
	A solid is a physical primitive that has material and can be textured
	*/
	class Solid : public Primitive
	{
	public:
		CoordMapper* tex_mapper_;
		Material* material_;

	public:
		explicit Solid(CoordMapper* tex_mapper = nullptr, Material* material = nullptr)
			: tex_mapper_(tex_mapper)
			, material_(material)
		{
			if (tex_mapper_ == nullptr)
			{
				tex_mapper_ = new WorldMapper(Float4(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		virtual Point sample() const = 0;
		virtual float getArea() const = 0;
		virtual void setMaterial(Material* m) { material_ = m; }
		virtual void setCoordMapper(CoordMapper* cm)
		{
			tex_mapper_ = cm;

			if (tex_mapper_ == nullptr)
			{
				tex_mapper_ = new WorldMapper(Float4(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
	};
}

#endif