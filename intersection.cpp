#include <rt/intersection.h>

using namespace rt;

//ctor
Intersection::Intersection()
	: b_intersection_(false)
{}

//ctor
Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local, bool b_intersection)
	: distance_(distance)
	, ray_(ray)
	, solid_(solid)
	, normal_(normal)
	, local_(local)
	, b_intersection_(b_intersection)
{}

//failure
Intersection Intersection::failure()
{
	return Intersection();
}

//hitPoint
Point Intersection::hitPoint() const
{
	return ray_.o_ + distance_*ray_.d_;
}

//normal
Vector Intersection::normal() const
{
	return normal_;
}

//local
Point Intersection::local() const
{
	return local_;
}

//operator bool
Intersection::operator bool()
{
	return b_intersection_;
}