#include <core/point.h>
#include <core/vector.h>
#include <core/assert.h>
#include <core/float4.h>

using namespace rt;

//ctor
//CURRENTLY NOT IMPLEMENTED
Point::Point(const Float4& f4)
{
	assert(f4[3] != 0) << "ERROR - CONVERSION FROM FLOAT4 TO POINT";

	x_ = f4[0] / f4[3];
	y_ = f4[1] / f4[3];
	z_ = f4[2] / f4[3];
}

//operator Point-Point
Vector Point::operator-(const Point& p) const
{
	return Vector(x_ - p.x_, y_ - p.y_, z_ - p.z_);
}

//operator ==
bool Point::operator==(const Point& p) const
{
	return x_ == p.x_ && y_ == p.y_ && z_ == p.z_;
}

//operator !=
bool Point::operator!=(const Point& p) const
{
	return !(*this == p);
}

//operator float*Point
Point rt::operator*(float scalar, const Point& p)
{
	return Point(scalar*p.x_, scalar*p.y_, scalar*p.z_);
}

//operator Point*float
Point rt::operator*(const Point& p, float scalar)
{
	return Point(scalar*p.x_, scalar*p.y_, scalar*p.z_);
}

//min
Point rt::min(const Point& p, const Point& o)
{
	return Point(p.x_ < o.x_ ? p.x_ : o.x_, p.y_ < o.y_ ? p.y_ : o.y_, p.z_ < o.z_ ? p.z_ : o.z_);
}

//max
Point rt::max(const Point& p, const Point& o)
{
	return Point(p.x_ > o.x_ ? p.x_ : o.x_, p.y_ > o.y_ ? p.y_ : o.y_, p.z_ > o.z_ ? p.z_ : o.z_);
}