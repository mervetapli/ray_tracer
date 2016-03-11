#include <rt/solids/quad.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/random.h>
#include <core/interpolate.h>

using namespace rt;

//ctor
Quad::Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, v1_(v1)
	, span1_(span1)
	, span2_(span2)
{}

//getBounds
BBox Quad::getBounds() const
{
	Point v2 = v1_ + span1_;
	Point v3 = v1_ + span2_;
	Point v4 = v1_ + span1_ + span2_;

	Point min = rt::min(v1_, v2);
	min = rt::min(min, v3);
	min = rt::min(min, v4);

	Point max = rt::max(v1_, v2);
	max = rt::max(max, v3);
	max = rt::max(max, v4);

	return BBox(min, max);
}

//intersect
Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const
{
	Vector normal = cross(span2_, span1_).normalize();

	float x = dot(ray.d_, normal);

	if (std::abs(x)<epsilon)
	{
		return Intersection::failure();
	}

	float distance = (dot(Vector(v1_.x_, v1_.y_, v1_.z_), normal) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal)) / x;
	Vector local_position = ray.getPoint(distance) - v1_;

	//local_position is the local position vector of the hit point.
	//in order to understand if hit point lies on the quad or not,
	//project local_position vector onto both span1 and span2.
	//then compare the square of the lengths
	float u = dot(span1_.normalize(), local_position);
	float v = dot(span2_.normalize(), local_position);

	if (u >= 0 && u*u <= dot(span1_, span1_) && v >= 0 && v*v <= dot(span2_, span2_))
	{
		return Intersection(distance, ray, this, normal, Point(u, v, 0), true);
	}

	else
	{
		return Intersection::failure();
	}
}

//sample
Point Quad::sample() const
{
	return v1_ + span1_*random() + span2_*random();
}

//getArea
float Quad::getArea() const
{
	return cross(span1_, span2_).length();
}