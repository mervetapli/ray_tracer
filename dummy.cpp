#include <rt/materials/dummy.h>

using namespace rt;

//ctor
DummyMaterial::DummyMaterial()
	: reflectance_(1.0f, 1.0f, 1.0f)
{}

//ctor
RGBColor DummyMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	//be sure that normal is already normalized.
	float cos_theta = dot(in_dir, normal);

	return reflectance_*cos_theta;
}

//getEmission
RGBColor DummyMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	//emits no light
	return RGBColor(0.0f, 0.0f, 0.0f);
}

//getSampleReflectance
Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return SampleReflectance();
}