#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt
{
	class Intersection;

	class CylindricalCoordMapper : public CoordMapper
	{
	public:
		CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
		virtual Point getCoords(const Intersection& hit) const;

	private:
		Point origin_;
		Vector u_, v_, w_;
		float x_len_, y_len_;
	};
}

#endif