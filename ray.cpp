#include <rt/ray.h>

using namespace rt;

//ctor
Ray::Ray(const Point& o, const Vector& p)
	: o_(o)
	, d_(p)
{}

//getPoint
Point Ray::getPoint(float distance) const
{
	return o_ + distance*d_.normalize();
}