#include <rt/coordmappers/spherical.h>
#include <core/assert.h>
#include <core/scalar.h>
#include <rt/intersection.h>

using namespace rt;

//ctor
SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
	: origin_(origin)
{
	w_ = zenith.normalize();
	v_ = cross(w_, azimuthRef).normalize();
	u_ = cross(v_, w_).normalize();

	//zenith and azimuthref in uvw space.
	zenith_ = Vector(dot(zenith, u_), dot(zenith, v_), dot(zenith, w_));
	x_len_ = azimuthRef.length();
	y_len_ = zenith.length();
}

//getCoords
Point SphericalCoordMapper::getCoords(const Intersection& hit) const
{
	Vector local = hit.local() - origin_;

	//specify 'local' in uvw space
	local = Vector(dot(local, u_), dot(local, v_), dot(local, w_));

	float phi = -atan2(local.y_, local.x_) / (2 * pi);
	float cos_theta = dot(local.normalize(), zenith_.normalize());
	float theta = acos(cos_theta) / pi;

	return Point(phi / x_len_, theta / y_len_, 0.0f);
}