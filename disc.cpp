#include <rt/solids/disc.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <core/random.h>
#include <rt/primmod/instance.h>

using namespace rt;

//ctor
Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* tex_mapper, Material* material)
	: Solid(tex_mapper, material)
	, center_(center)
	, normal_(normal.normalize())
	, radius_(radius)
{}

//getBounds
BBox Disc::getBounds() const
{
	//be aware that this is not the tightest bounding box for disc
	return BBox(Point(center_.x_ - radius_, center_.y_ - radius_, center_.z_ - radius_),
		Point(center_.x_ + radius_, center_.y_ + radius_, center_.z_ + radius_));
}

//intersect
Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const
{
	//same as ray-plane intersection calculations
	float x = dot(ray.d_, normal_);

	if (std::abs(x)<epsilon)
	{
		return Intersection::failure();
	}

	float distance = (dot(Vector(center_.x_, center_.y_, center_.z_), normal_) - dot(Vector(ray.o_.x_, ray.o_.y_, ray.o_.z_), normal_)) / x;
	Vector local_position = ray.getPoint(distance) - center_;

	//local_position is the local position vector of the hit point.
	//if it is less than radius of the disc, there is an intersection.
	//compare the square instead of lenght for more efficient calculation
	if (dot(local_position, local_position) <= radius_*radius_)
	{
		return Intersection(distance, ray, this, normal_, ray.getPoint(distance), true);
	}

	else
	{
		return Intersection::failure();
	}
}

//sample
Point Disc::sample() const
{
	Vector dir(-normal_.y_, normal_.x_, 0);
	float distance = radius_*random();
	float angle = 2 * pi*random();

	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float k = 1 - cos;

	Matrix rotation(
		Float4(cos + normal_.x_*normal_.x_*k, normal_.x_*normal_.y_*k - normal_.z_*sin, normal_.x_*normal_.z_*k + normal_.y_*sin, 0.0f),
		Float4(normal_.y_*normal_.x_*k + normal_.z_*sin, cos + normal_.y_*normal_.y_*k, normal_.y_*normal_.z_*k - normal_.x_*sin, 0.0f),
		Float4(normal_.z_*normal_.x_*k - normal_.y_*sin, normal_.z_*normal_.y_*k + normal_.x_*sin, cos + normal_.z_*normal_.z_*k, 0.0f),
		Float4(0.0f, 0.0f, 0.0f, 1.0f));

	dir = Vector(rotation*Float4(dir)).normalize();

	return center_ + dir*distance;
}

//getArea
float Disc::getArea() const
{
	return pi*radius_*radius_;
}