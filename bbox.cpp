#include <rt/bbox.h>
#include <rt/ray.h>

using namespace rt;

//empty
BBox BBox::empty()
{
	//return a negative box
	return BBox(Point(FLT_MAX, FLT_MAX, FLT_MAX), Point(-FLT_MAX, -FLT_MAX, -FLT_MAX));
}

//full
BBox BBox::full()
{
	return BBox(Point(-FLT_MAX, -FLT_MAX, -FLT_MAX), Point(FLT_MAX, FLT_MAX, FLT_MAX));
}

//extend
void BBox::extend(const Point& point)
{
	min_ = rt::min(point, min_);
	max_ = rt::max(point, max_);
}

//extend
void BBox::extend(const BBox& bbox)
{
	min_ = rt::min(bbox.min_, min_);
	max_ = rt::max(bbox.max_, max_);
}

//intersect
std::pair<float, float> BBox::intersect(const Ray& ray) const
{
	float tmin = (min_.x_ - ray.o_.x_) / ray.d_.x_;
	float tmax = (max_.x_ - ray.o_.x_) / ray.d_.x_;

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	float tymin = (min_.y_ - ray.o_.y_) / ray.d_.y_;
	float tymax = (max_.y_ - ray.o_.y_) / ray.d_.y_;

	if (tymin > tymax)
	{
		std::swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return std::make_pair(1.0f, 0.0f);
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (min_.z_ - ray.o_.z_) / ray.d_.z_;
	float tzmax = (max_.z_ - ray.o_.z_) / ray.d_.z_;

	if (tzmin > tzmax)
	{
		std::swap(tzmin, tzmax);
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return std::make_pair(1.0f, 0.0f);
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	return std::make_pair(tmin, tmax);
}

//isUnbound
bool BBox::isUnbound()
{
	if (std::abs(min_.x_ + FLT_MAX) < epsilon && std::abs(max_.x_ - FLT_MAX) < epsilon)
	{
		return true;
	}

	if (std::abs(min_.y_ + FLT_MAX) < epsilon && std::abs(max_.y_ - FLT_MAX) < epsilon)
	{
		return true;
	}

	if (std::abs(min_.z_ + FLT_MAX) < epsilon && std::abs(max_.z_ - FLT_MAX) < epsilon)
	{
		return true;
	}

	return false;
}


float BBox::getArea() const
{
	float x = max_.x_ - min_.x_;
	float y = max_.y_ - min_.y_;
	float z = max_.z_ - min_.z_;

	return 2 * (x*y + x*z + y*z);
}