#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt
{
	class Triangle : public Solid
	{
	public:
		Triangle() {}
		Triangle(Point vertices[3], CoordMapper* tex_mapper, Material* material);
		Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* tex_mapper, Material* material);

		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual Point sample() const;
		virtual float getArea() const;

	public:
		Point v1_, v2_, v3_;
	};
}

#endif