#include "gradient.h"
#include <core/assert.h>
#include <core/vector.h>

using namespace rt;

GradientTexture::GradientTexture(const RGBColor& rgb1, const RGBColor& rgb2, const float height)
	: white_(rgb1)
	, black_(rgb2)
	, height_(height)
{}
RGBColor GradientTexture::getColor(const Point& coord)
{
	return (white_*(coord.y_ - height_) + black_*coord.y_) / 0.5;
}
RGBColor GradientTexture::getColorDX(const Point& coord)
{
	NOT_IMPLEMENTED;
}
RGBColor GradientTexture::getColorDY(const Point& coord)
{
	NOT_IMPLEMENTED;
}