#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt
{
	class Disc : public Solid
	{
	public:
		Disc() {}
		Disc(const Point& center, const Vector& normal, float radius, CoordMapper* tex_mapper, Material* material);

		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual Point sample() const;
		virtual float getArea() const;

	private:
		Point center_;
		Vector normal_;
		float radius_;
	};
}

#endif