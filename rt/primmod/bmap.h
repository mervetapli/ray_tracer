#ifndef CG1RAYTRACER_PRIMMOD_BMAP_HEADER
#define CG1RAYTRACER_PRIMMOD_BMAP_HEADER

#include <vector>
#include <rt/primitive.h>
#include <core/point.h>

namespace rt
{
	class Triangle;
	class Texture;

	class BumpMapper : public Primitive
	{
	public:
		BumpMapper(Triangle* base, Texture* bump_map, const Point& v1, const Point& v2, const Point& v3, float vscale);
		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual void setMaterial(Material* m);
		virtual void setCoordMapper(CoordMapper* cm);

	private:
		Triangle* base_;
		Texture* bump_map_;
		Point v1_, v2_, v3_;
		Vector u_, v_;
		float scale_;
	};
}

#endif