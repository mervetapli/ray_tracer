#include <core/float4.h>
#include <iostream>
#include <core/point.h>
#include <core/vector.h>
#include <core/assert.h>

using namespace rt;

//ctor
Float4::Float4(float x, float y, float z, float w)
{
	dim[0] = x;
	dim[1] = y;
	dim[2] = z;
	dim[3] = w;
}

//ctor
Float4::Float4(const Point& p)
{
	dim[0] = p.x_;
	dim[1] = p.y_;
	dim[2] = p.z_;
	dim[3] = 1.0f;
}

//ctor
Float4::Float4(const Vector& v)
{
	dim[0] = v.x_;
	dim[1] = v.y_;
	dim[2] = v.z_;
	dim[3] = 0.0f;
}

//index operator
float& Float4::operator[](int index)
{
	return dim[index];
}

//index operator
float Float4::operator[](int index) const
{
	return dim[index];
}

//operator Float4+Float4
Float4 Float4::operator+(const Float4& b) const
{
	return Float4(dim[0] + b[0], dim[1] + b[1], dim[2] + b[2], dim[3] + b[3]);
}

//operator Float4-Float4
Float4 Float4::operator-(const Float4& b) const
{
	return Float4(dim[0] - b[0], dim[1] - b[1], dim[2] - b[2], dim[3] - b[3]);
}

//operator Float4*Float4
Float4 Float4::operator*(const Float4& b) const
{
	return Float4(dim[0] * b[0], dim[1] * b[1], dim[2] * b[2], dim[3] * b[3]);
}

//operator Float4/Float4
Float4 Float4::operator/(const Float4& b) const
{
	assert(b[0] == 0 || b[1] == 0 || b[2] == 0 || b[3] == 0) << "Division by 0";
	return Float4(dim[0] / b[0], dim[1] / b[1], dim[2] / b[2], dim[3] / b[3]);
}

//operator -Float4
Float4 Float4::operator-() const
{
	return Float4(-dim[0], -dim[1], -dim[2], -dim[3]);
}

//operator ==
bool Float4::operator==(const Float4& b) const
{
	return dim[0] == b[0] && dim[1] == b[1] && dim[2] == b[2] && dim[3] == b[3];
}

//operator !=
bool Float4::operator!=(const Float4& b) const
{
	return !(*this == b);
}

//operator scalar*Float4
Float4 rt::operator*(float scalar, const Float4& b)
{
	return Float4(scalar * b[0], scalar * b[1], scalar * b[2], scalar * b[3]);
}

//operator Float4*scalar
Float4 rt::operator*(const Float4& b, float scalar)
{
	return Float4(b[0] * scalar, b[1] * scalar, b[2] * scalar, b[3] * scalar);
}

//operator Float4/scalar
Float4 rt::operator/(const Float4& a, float scalar)
{
	assert(scalar != 0) << "Division by 0";
	return Float4(a[0] / scalar, a[1] / scalar, a[2] / scalar, a[3] / scalar);
}

//dot product
float rt::dot(const Float4& a, const Float4& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

//min
Float4 rt::min(const Float4& a, const Float4& b)
{
	return Float4(a[0] < b[0] ? a[0] : b[0], a[1] < b[1] ? a[1] : b[1],
		a[2] < b[2] ? a[2] : b[2], a[3] < b[3] ? a[3] : b[3]);
}

//max
Float4 rt::max(const Float4& a, const Float4& b)
{
	return Float4(a[0] > b[0] ? a[0] : b[0], a[1] > b[1] ? a[1] : b[1],
		a[2] > b[2] ? a[2] : b[2], a[3] > b[3] ? a[3] : b[3]);
}