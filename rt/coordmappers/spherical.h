#ifndef CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt
{
	class Intersection;
	class SphericalCoordMapper : public CoordMapper
	{
	public:
		SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef);
		virtual Point getCoords(const Intersection& hit) const;

	private:
		Point origin_;
		Vector zenith_;
		Vector u_, v_, w_;
		float x_len_, y_len_;
	};
}

#endif