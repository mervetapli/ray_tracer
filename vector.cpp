#include <core/vector.h>
#include <cmath>
#include <core/point.h>
#include <core/assert.h>
#include <core/float4.h>

using namespace rt;

//ctor
Vector::Vector(const Float4& f4)
{
	assert(f4[3] != 1) << "ERROR - CONVERSION FROM FLOAT4 TO VECTOR";

	x_ = f4[0];
	y_ = f4[1];
	z_ = f4[2];
}

//operation Vector+Vector
Vector Vector::operator+(const Vector& v) const
{
	return Vector(x_ + v.x_, y_ + v.y_, z_ + v.z_);
}

//operation Vector-Vector
Vector Vector::operator-(const Vector& v) const
{
	return Vector(x_ - v.x_, y_ - v.y_, z_ - v.z_);
}

//normalize
Vector Vector::normalize() const
{
	return (*this) / length();
}

//operation -Vector
Vector Vector::operator-() const
{
	return (*this)*(-1);
}

//lensqr
float Vector::lensqr() const
{
	return x_*x_ + y_*y_ + z_*z_;
}

//length
float Vector::length() const
{
	return sqrt(lensqr());
}

//operator ==
bool Vector::operator==(const Vector& v) const
{
	return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
}

//operator !=
bool Vector::operator!=(const Vector& v) const
{
	return !(*this == v);
}

//operator float*Vector
Vector rt::operator*(float scalar, const Vector& v)
{
	return Vector(scalar*v.x_, scalar*v.y_, scalar*v.z_);
}

//operator Vector*float
Vector rt::operator*(const Vector& v, float scalar)
{
	return Vector(v.x_*scalar, v.y_*scalar, v.z_*scalar);
}

//operator Vector/float
Vector rt::operator/(const Vector& v, float scalar)
{
	assert(scalar != 0) << "Division by 0";
	return Vector(v.x_ / scalar, v.y_ / scalar, v.z_ / scalar);
}

//cross product
Vector rt::cross(const Vector& u, const Vector& v)
{
	return Vector(u.y_*v.z_ - u.z_*v.y_, u.z_*v.x_ - u.x_*v.z_, u.x_*v.y_ - u.y_*v.x_);
}

//dot product
float rt::dot(const Vector& u, const Vector& v)
{
	return u.x_*v.x_ + u.y_*v.y_ + u.z_*v.z_;
}

//min
Vector rt::min(const Vector& u, const Vector& v)
{
	return Vector(u.x_ < v.x_ ? u.x_ : v.x_, u.y_ < v.y_ ? u.y_ : v.y_, u.z_ < v.z_ ? u.z_ : v.z_);
}

//max
Vector rt::max(const Vector& u, const Vector& v)
{
	return Vector(u.x_ > v.x_ ? u.x_ : v.x_, u.y_ > v.y_ ? u.y_ : v.y_, u.z_ > v.z_ ? u.z_ : v.z_);
}

//operator Point+Vector
Point rt::operator+(const Point& p, const Vector& v)
{
	return Point(p.x_ + v.x_, p.y_ + v.y_, p.z_ + v.z_);
}

//operator Vector+Point
Point rt::operator+(const Vector& v, const Point& p)
{
	return Point(v.x_ + p.x_, v.y_ + p.y_, v.z_ + p.z_);
}

//operator Point-Vector
Point rt::operator-(const Point& p, const Vector& v)
{
	return Point(p.x_ - v.x_, p.y_ - v.y_, p.z_ - v.z_);
}

//operator Float4*Point
//CURRENTLY NOT IMPLEMENTED
Point rt::operator*(const Float4& scale, const Point& p)
{
	return Point();
}