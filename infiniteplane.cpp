#include <rt/solids/infiniteplane.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/assert.h>

using namespace rt;

//ctor
InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, origin_(origin)
	, normal_(normal.normalize())
{}

//getBounds
BBox InfinitePlane::getBounds() const
{
	return BBox::full();
}

//intersect
Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const
{
	float x = dot(ray.d_, normal_);

	if (std::abs(x)<epsilon)
	{
		return Intersection::failure();
	}

	float distance = (dot(Vector(origin_.x_, origin_.y_, origin_.z_), normal_) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal_)) / x;

	return Intersection(distance, ray, this, normal_, ray.getPoint(distance), true);
}

//sample
Point InfinitePlane::sample() const
{
	NOT_IMPLEMENTED;
}

//getArea
//not sensible to compute since it is infinite
float InfinitePlane::getArea() const
{
	return FLT_MAX;
}