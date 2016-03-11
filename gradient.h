#ifndef CG1RAYTRACER_TEXTURES_GRADIENT_HEADER
#define CG1RAYTRACER_TEXTURES_GRADÝENT_HEADER

#include <vector>
#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/point.h>

using namespace rt;

class GradientTexture : public Texture
{
public:
	GradientTexture(const RGBColor& rgb1, const RGBColor& rgb2, const float height);

	virtual RGBColor getColor(const Point& coord);
	virtual RGBColor getColorDX(const Point& coord);
	virtual RGBColor getColorDY(const Point& coord);
private:
	RGBColor white_, black_;
	float height_;
};

#endif

