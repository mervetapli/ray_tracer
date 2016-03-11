#include <rt/solids/sphere.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/assert.h>

using namespace rt;

//ctor
Sphere::Sphere(const Point& center, float radius, CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, center_(center)
	, radius_(radius)
{}

//getBounds
BBox Sphere::getBounds() const
{
	return BBox(Point(center_.x_ - radius_, center_.y_ - radius_, center_.z_ - radius_),
		Point(center_.x_ + radius_, center_.y_ + radius_, center_.z_ + radius_));
}

//intersect
Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const
{
	//geometrical approach
	Vector e = center_ - ray.o_;
	float a = dot(e, ray.d_);
	float x = radius_*radius_ + a*a - dot(e, e);
	
	if (x < 0)
	{
		return Intersection::failure();
	}

	float distance = a - std::sqrt(x);

	return Intersection(distance, ray, this, (ray.getPoint(distance) - center_).normalize(), ray.getPoint(distance), true);
}

//sample
Point Sphere::sample() const
{
	NOT_IMPLEMENTED;
}

//getArea
float Sphere::getArea() const
{
	return 4*pi*radius_*radius_;
}