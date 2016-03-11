#ifndef CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER
#define CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER

#include <rt/solids/solid.h>
#include <core/float4.h>
#include <core/point.h>

namespace rt
{
	class InfinitePlane : public Solid
	{
	public:
		InfinitePlane() {}
		InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* tex_mapper, Material* material);

		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual Point sample() const;
		virtual float getArea() const;

	private:
		Point origin_;
		Vector normal_;
	};
}

#endif