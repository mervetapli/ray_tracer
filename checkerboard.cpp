#include <rt/textures/checkerboard.h>
#include <core/point.h>
#include <core/assert.h>

using namespace rt;

//ctor
CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
	: white_(white)
	, black_(black)
{}

//getColor
RGBColor CheckerboardTexture::getColor(const Point& coord)
{
	bool u = coord.x_ - floor(coord.x_) < 0.5f;
	bool v = coord.y_ - floor(coord.y_) < 0.5f;
	bool w = coord.z_ - floor(coord.z_) < 0.5f;

	if (u ^ v ^ w)
		return white_;
	else
		return black_;
}

//getColorDX
RGBColor CheckerboardTexture::getColorDX(const Point& coord)
{
	NOT_IMPLEMENTED;
}

//getColorDY
RGBColor CheckerboardTexture::getColorDY(const Point& coord)
{
	NOT_IMPLEMENTED;
}