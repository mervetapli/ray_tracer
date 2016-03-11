#include <rt/solids/aabox.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/assert.h>

using namespace rt;

//ctor
AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, corner1_(corner1)
	, corner2_(corner2)
{}

//getBounds
BBox AABox::getBounds() const
{
	return BBox(rt::min(corner1_, corner2_), rt::max(corner1_, corner2_));
}

//intersect
Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const
{
	Point min = rt::min(corner1_, corner2_);
	Point max = rt::max(corner1_, corner2_);

	//can intersect x=0 or x=x0 plane
	if (std::abs(ray.d_.x_) > epsilon)
	{
		Vector normal(ray.d_.x_ > 0 ? -1.0f : 1.0f, 0.0f, 0.0f);
		Vector vec = ray.d_.x_ > 0 ? Vector(min.x_, min.y_, min.z_) : Vector(max.x_, max.y_, max.z_);
		float distance = (dot(vec, normal) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal)) / dot(ray.d_, normal);

		Point p = ray.getPoint(distance);
		if (min.y_ <= p.y_ && p.y_ <= max.y_ && min.z_ <= p.z_ && p.z_ <= max.z_)
		{
			Vector local_position = ray.getPoint(distance) - (min + (max - min) / 2);
			Intersection temp(distance, ray, this, normal, Point(local_position.x_, local_position.y_, local_position.z_));
			temp.b_intersection_ = true;

			return temp;
		}
	}

	//can intersect y=0 or y=y0 plane
	if (std::abs(ray.d_.y_) > epsilon)
	{
		Vector normal(0.0f, ray.d_.y_ > 0 ? -1.0f : 1.0f, 0.0f);
		Vector vec = ray.d_.y_ > 0 ? Vector(min.x_, min.y_, min.z_) : Vector(max.x_, max.y_, max.z_);
		float distance = (dot(vec, normal) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal)) / dot(ray.d_, normal);

		Point p = ray.getPoint(distance);
		if (min.x_ <= p.x_ && p.x_ <= max.x_ && min.z_ <= p.z_ && p.z_ <= max.z_)
		{
			Vector local_position = ray.getPoint(distance) - (min + (max - min) / 2);
			Intersection temp(distance, ray, this, normal, Point(local_position.x_, local_position.y_, local_position.z_));
			temp.b_intersection_ = true;

			return temp;
		}
	}

	//can intersect z=0 or z=z0 plane
	if (std::abs(ray.d_.z_) > epsilon)
	{
		Vector normal(0.0f, 0.0f, ray.d_.z_ > 0 ? -1.0f : 1.0f);
		Vector vec = ray.d_.z_ > 0 ? Vector(min.x_, min.y_, min.z_) : Vector(max.x_, max.y_, max.z_);
		float distance = (dot(vec, normal) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal)) / dot(ray.d_, normal);

		Point p = ray.getPoint(distance);
		if (min.x_ <= p.x_ && p.x_ <= max.x_ && min.y_ <= p.y_ && p.y_ <= max.y_)
		{
			Vector local_position = ray.getPoint(distance) - (min + (max - min) / 2);
			Intersection temp(distance, ray, this, normal, Point(local_position.x_, local_position.y_, local_position.z_));
			temp.b_intersection_ = true;

			return temp;
		}
	}

	return Intersection::failure();
}

//sample
Point AABox::sample() const
{
	NOT_IMPLEMENTED;
}

//getArea
float AABox::getArea() const
{
	Point min = rt::min(corner1_, corner2_);
	Point max = rt::max(corner1_, corner2_);

	float x = max.x_ - min.x_;
	float y = max.y_ - min.y_;
	float z = max.z_ - min.z_;

	return 2 * (x*y + x*z + y*z);
}