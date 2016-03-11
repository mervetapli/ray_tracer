#include <rt/lights/spotlight.h>
#include <core/color.h>
#include <algorithm>

using namespace rt;

//ctor
SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float exp, const RGBColor& intensity)
	: position_(position)
	, direction_(direction.normalize())
	, angle_(angle)
	, exp_(exp)
	, intensity_(intensity.r, intensity.g, intensity.b)
{}

//getLightHit
LightHit SpotLight::getLightHit(const Point& p) const
{
	Vector dir = position_ - p;
	return LightHit(dir.normalize(), dir.length());
}

//getIntensity
RGBColor SpotLight::getIntensity(const LightHit& irr) const
{
	float cos_theta = dot(-irr.direction.normalize(), direction_.normalize());

	if (std::acos(cos_theta) > angle_)
	{
		return RGBColor::rep(0.0f);
	}

	return intensity_ / (irr.distance*irr.distance) * std::pow(cos_theta, exp_);
}