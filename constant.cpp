#include <rt/textures/constant.h>

using namespace rt;

//ctor
ConstantTexture::ConstantTexture(const RGBColor& color)
	: color_(color)
{}

//getColor
RGBColor ConstantTexture::getColor(const Point& coord)
{
	return color_;
}

//getColorDX
RGBColor ConstantTexture::getColorDX(const Point& coord)
{
	return color_;
}

//getColorDY
RGBColor ConstantTexture::getColorDY(const Point& coord)
{
	return color_;
}