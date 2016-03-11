#ifndef CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

	class Intersection;
	class EnvironmentMapper : public CoordMapper {
	private:
		Point sOrigin;
		Vector sZenith;
		Vector sReferenceAzimuth;
		Vector X, Y, Z;
		float mInitialPhasePhi;

	public:
		EnvironmentMapper(Vector& zenith, Vector& referenceAzimuth);
		virtual Point getCoords(const Intersection& hit) const;
	};

}


#endif
