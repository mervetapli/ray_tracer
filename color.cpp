#include <core/color.h>
#include <core/assert.h>

using namespace rt;

//ctor
RGBColor::RGBColor(const Float4& f4)
{}

//operator RGBColor+RGBColor
RGBColor RGBColor::operator+(const RGBColor& c) const
{
	return RGBColor(r + c.r, g + c.g, b + c.b);
}

//operator RGBColor-RGBColor
RGBColor RGBColor::operator-(const RGBColor& c) const
{
	return RGBColor(r - c.r, g - c.g, b - c.b);
}

//operator RGBColor*RGBColor
RGBColor RGBColor::operator*(const RGBColor& c) const
{
	return RGBColor(r * c.r, g * c.g, b * c.b);
}

//operator ==
bool RGBColor::operator==(const RGBColor& c) const
{
	return r == c.r && g == c.g && b == c.b;
}

//operator !=
bool RGBColor::operator!=(const RGBColor& c) const
{
	return !(*this == c);
}

//clamp
RGBColor RGBColor::clamp() const
{
	return (*this) / 255;
}

//gamma
//CURRENTLY NOT IMPLEMENTED
RGBColor RGBColor::gamma(float gam) const
{
	return RGBColor();
}

//luminance
//CURRENTLY NOT IMPLEMENTED
float RGBColor::luminance() const
{
	return 0.0f;
}

//operator float*RGBColor
RGBColor rt::operator*(float scalar, const RGBColor& c)
{
	return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

//operator RGBColor*float
RGBColor rt::operator*(const RGBColor& c, float scalar)
{
	return RGBColor(c.r*scalar, c.g*scalar, c.b*scalar);
}

//operator RGBColor/float
RGBColor rt::operator/(const RGBColor& c, float scalar)
{
	assert(scalar != 0) << "Division by 0";
	return RGBColor(c.r / scalar, c.g / scalar, c.b / scalar);
}