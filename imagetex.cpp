#include <rt/textures/imagetex.h>
#include <algorithm>
#include <core/point.h>

using namespace rt;

//ctor
ImageTexture::ImageTexture()
{}

//ctor
ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
	: bh_type_(bh)
	, i_type_(i)
	, image_(image)
{}

//ctor
ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
	: bh_type_(bh)
	, i_type_(i)
{
	image_.readPNG(filename);
}

//getColor
RGBColor ImageTexture::getColor(const Point& coord)
{
	uint resolution_u = image_.width();
	uint resolution_v = image_.height();

	//fractional coordinates
	float tu, tv;

	//decide how to handle borders.
	switch (bh_type_)
	{
	case REPEAT:
		tu = coord.x_ - floor(coord.x_);
		tv = coord.y_ - floor(coord.y_);
		break;

	case MIRROR:
	{
		int lu = floor(coord.x_);
		int lv = floor(coord.y_);
		tu = coord.x_ - lu;
		tv = coord.y_ - lv;

		if (std::abs(lu) % 2 == 1)
			tu = 1 - tu;

		if (std::abs(lv) % 2 == 1)
			tv = 1 - tv;
	}
		break;

	case CLAMP:
		if (coord.x_ < 0)
			tu = 0;
		else if (coord.x_ > 1)
			tu = 1;
		else
			tu = coord.x_;

		if (coord.y_ < 0)
			tv = 0;
		else if (coord.y_ > 1)
			tv = 1;
		else
			tv = coord.y_;
		break;
	}

	//decide how to interpolate
	if (i_type_ == BILINEAR)
	{
		float u = tu*(resolution_u - 2);
		float v = tv*(resolution_v - 2);

		uint bottom_u = static_cast<uint>(u);
		uint bottom_v = static_cast<uint>(v);
		float fu = u - bottom_u;
		float fv = v - bottom_v;

		return (1 - fu)*(1 - fv)*image_(bottom_u, bottom_v)
			+ (1 - fu)*(fv)*image_(bottom_u, bottom_v + 1)
			+ (fu)*(1 - fv)*image_(bottom_u + 1, bottom_v)
			+ (fu)*(fv)*image_(bottom_u + 1, bottom_v + 1);
	}

	//i_type==NEAREST
	else
	{
		float u = tu*(resolution_u-1);
		float v = tv*(resolution_v-1);

		return image_(static_cast<uint>(u), static_cast<uint>(v));
	}
}

//getColorDX
RGBColor ImageTexture::getColorDX(const Point& coord)
{
	return getColor(Point(coord.x_ - 0.5f / image_.width(), coord.y_, 0.0f)) - getColor(Point(coord.x_ + 0.5f / image_.width(), coord.y_, 0.0f));
}

//getColorDY
RGBColor ImageTexture::getColorDY(const Point& coord)
{
	return getColor(Point(coord.x_, coord.y_ - 0.5f / image_.height(), 0.0f)) - getColor(Point(coord.x_, coord.y_ + 0.5f / image_.height(), 0.0f));
}