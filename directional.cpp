#include <rt/lights/directional.h>
#include <rt/lights/light.h>
#include <core/color.h>
#include <core/assert.h>
#include <algorithm>

using namespace rt;

//ctor
DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& intensity)
	: direction_(direction.normalize())
	, intensity_(intensity.r, intensity.g, intensity.b)
{}

//getLightHit
LightHit DirectionalLight::getLightHit(const Point& p) const
{
	return LightHit(-direction_, FLT_MAX);
}

//getIntensity
RGBColor DirectionalLight::getIntensity(const LightHit& irr) const
{
	return intensity_;
}