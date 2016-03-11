#include <rt/coordmappers/environmentmap.h>
#include <rt/intersection.h>

using namespace rt;

EnvironmentMapper::EnvironmentMapper(Vector& zenith, Vector& referenceAzimuth) 
{
	Z = zenith.normalize();
	Vector tmpX = (abs(Z.x_) >= 1) ? Vector(0, 1, 0) : Vector(1, 0, 0);
	Y = cross(Z, tmpX).normalize();
	X = cross(Y, Z).normalize();
	sZenith = Vector(dot(zenith, X), dot(zenith, Y), dot(zenith, Z));
	sReferenceAzimuth = Vector(dot(referenceAzimuth, X), dot(referenceAzimuth, Y), dot(referenceAzimuth, Z));
	mInitialPhasePhi = atan2(sReferenceAzimuth.y_, sReferenceAzimuth.x_);

	if (mInitialPhasePhi < 0) mInitialPhasePhi = mInitialPhasePhi + 2 * pi;
}

Point EnvironmentMapper::getCoords(const Intersection& hit) const
{
	Point local = hit.local();
	Vector p = local - sOrigin;
	Vector v = Vector(dot(p, X), dot(p, Y), dot(p, Z));
	Vector s = v;
	s.z_ = 0;
	s = s.normalize();
	float phi = atan2(s.y_, s.x_) + mInitialPhasePhi;
	if (phi > 2 * pi) phi = phi - 2 * pi;
	float cosTheta = dot(v.normalize(), sZenith.normalize());
	float theta = acos(cosTheta);
	return Point(phi / (2 * pi), theta / (pi), 0);
}