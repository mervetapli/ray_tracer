#include <rt/coordmappers/cylindrical.h>
#include <core/assert.h>
#include <core/scalar.h>
#include <rt/intersection.h>

using namespace rt;

//ctor
CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
	: origin_(origin)
{
	w_ = longitudinalAxis.normalize();
	v_ = cross(w_, polarAxis).normalize();
	u_ = cross(v_, w_).normalize();

	x_len_ = polarAxis.length();
	y_len_ = longitudinalAxis.length();
}

//getCoords
Point CylindricalCoordMapper::getCoords(const Intersection& hit) const
{
	Vector local = hit.local() - origin_;

	//specify 'local' in uvw space
	local = Vector(dot(local, u_), dot(local, v_), dot(local, w_));

	float phi = -atan2(local.y_, local.x_) / (2 * pi);
	return Point(phi / x_len_, local.z_ / y_len_, 0.0f);
}