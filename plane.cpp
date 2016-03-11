#include <rt/coordmappers/plane.h>
#include <core/assert.h>
#include <core/scalar.h>
#include <rt/intersection.h>

using namespace rt;

//ctor
PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2)
	: e1_(e1)
	, e2_(e2)
{
	Vector e3 = cross(e1, e2);
	assert(cross(e1, e2).lensqr() > epsilon) << "Plane vectors should not be colinear";
	transform_ = Matrix::system(e1, e2, e3).invert();
}

//getCoords
Point PlaneCoordMapper::getCoords(const Intersection& hit) const
{
	return Point(transform_ * Float4(hit.local()));
}