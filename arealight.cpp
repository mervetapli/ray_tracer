#include <rt/lights/arealight.h>
#include <core/color.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <core/point.h>

using namespace rt;

//ctor
AreaLight::AreaLight(Solid* source)
	: source_(source)
{}

//getLightHit
LightHit AreaLight::getLightHit(const Point& p) const
{
	Vector dir = source_->sample() - p;
	return LightHit(dir.normalize(), dir.length());
}

//getIntensity
RGBColor AreaLight::getIntensity(const LightHit& irr) const
{
	return source_->material_->getEmission(Point(), Vector(), Vector()) / (irr.distance*irr.distance) * source_->getArea();
}