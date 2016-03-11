#include <rt/materials/phong.h>
#include <cmath>
#include <algorithm>
#include <core/scalar.h>

using namespace rt;

//ctor
PhongMaterial::PhongMaterial(Texture* specular, float exponent)
	: specular_(specular)
	, exponent_(exponent)
{}

//getReflectance
RGBColor PhongMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	//be sure that in_dir and normal are already normalized.
	Vector n = normal;
	float cosine = dot(in_dir, normal);
	if (cosine < 0.0)
	{
		cosine = -cosine;
		n = -normal;
	}
	Vector reflected = -in_dir + 2 * cosine * n;

	//division by (exponent_ + 2.0f) / (2.0f * pi) is for normalization.
	float specular_term = std::pow(std::max(0.0f, dot(reflected, out_dir)), exponent_) * ((exponent_ + 2.0f) / (2.0f * pi));
	return specular_->getColor(tex_point) * specular_term * cosine;
}

//getEmission
RGBColor PhongMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return RGBColor::rep(0.0f);
}

//getSampleReflectance
Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return SampleReflectance();
}

//useSampling
Material::Sampling PhongMaterial::useSampling() const
{
	return SAMPLING_NOT_NEEDED;
}