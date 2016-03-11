#include <rt/solids/triangle.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/random.h>
#include <core/interpolate.h>

using namespace rt;

//ctor
Triangle::Triangle(Point vertices[3], CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, v1_(vertices[0])
	, v2_(vertices[1])
	, v3_(vertices[2])
{}

//ctor
Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* tex_mapper, Material* material)
	: v1_(v1)
	, v2_(v2)
	, v3_(v3)
	, Solid(tex_mapper, material)
{}

//getBounds
BBox Triangle::getBounds() const
{
	return BBox(rt::min(rt::min(v1_, v2_), v3_), rt::max(rt::max(v1_, v2_), v3_));
}

//intersect
Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const
{
	//same as ray-plane intersection calculations.
	Vector normal = cross(v2_ - v1_, v3_ - v1_).normalize();
	float x = dot(ray.d_, normal);

	if (std::abs(x)<epsilon)
	{
		return Intersection::failure();
	}

	float distance = (dot(Vector(v1_.x_, v1_.y_, v1_.z_), normal) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal)) / x;

	//decide whether the point is inside the triangle or not.
	//define these variables static since they are remain same for every call
	Vector q1 = v2_ - v1_;
	Vector q2 = v3_ - v1_;
	float q1q1 = dot(q1, q1);
	float q2q2 = dot(q2, q2);
	float q1q2 = dot(q1, q2);

	Vector r = ray.getPoint(distance) - v1_;
	float rq1 = dot(r, q1);
	float rq2 = dot(r, q2);

	//calculate barycentric coordinates
	float w1 = (q2q2*rq1 - q1q2*rq2) / (q1q1*q2q2 - q1q2*q1q2);
	float w2 = (q1q1*rq2 - q1q2*rq1) / (q1q1*q2q2 - q1q2*q1q2);
	float w0 = 1 - w1 - w2;

	if (w0 >= 0 && w1 >= 0 && w2 >= 0)
	{
		return Intersection(distance, ray, this, normal, Point(w0, w1, w2), true);
	}

	return Intersection::failure();
}

//sample
Point Triangle::sample() const
{
	return lerpbar(v1_, v2_, v3_, random(), random());
}

//getArea
float Triangle::getArea() const
{
	Vector u = v2_ - v1_;
	Vector v = v3_ - v1_;

	return cross(u, v).length() / 2;
}