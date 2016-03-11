#include <rt/lights/pointlight.h>
#include <core/scalar.h>
#include <algorithm>

using namespace rt;

//ctor
PointLight::PointLight(const Point& position, const RGBColor& intensity)
	: position_(position)
	, intensity_(intensity.r, intensity.g, intensity.b)
{}

//getLightHit
LightHit PointLight::getLightHit(const Point& p) const
{
	Vector dir = position_ - p;
	return LightHit(dir.normalize(), dir.length());
}

//getIntensity
RGBColor PointLight::getIntensity(const LightHit& irr) const
{
	return intensity_ / (irr.distance*irr.distance);
}