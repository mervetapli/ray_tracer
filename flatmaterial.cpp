#include <rt/materials/flatmaterial.h>
#include <core/color.h>
#include <core/point.h>

using namespace rt;

//ctor
FlatMaterial::FlatMaterial(Texture* texture)
	: texture_(texture)
{}

//getReflectance
RGBColor FlatMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	//no reflectance
	return RGBColor::rep(0.0f);
}

//getEmission
RGBColor FlatMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return texture_->getColor(tex_point);
}

//getSampleReflectance
Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return SampleReflectance();
}