#include <rt/materials/lambertian.h>
#include <cmath>
#include <core/scalar.h>

using namespace rt;

//ctor
LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse)
	: emission_(emission)
	, diffuse_(diffuse)
{}

//getReflectance
RGBColor LambertianMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	//be sure that in_dir and normal are already normalized.
	float cosine = std::abs(dot(in_dir, normal));

	//division by pi is for normalization.
	return diffuse_->getColor(tex_point) * cosine / pi;
}

//getEmission
RGBColor LambertianMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return emission_->getColor(tex_point);
}

//getSampleReflectance
Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return SampleReflectance();
}

//useSampling
Material::Sampling LambertianMaterial::useSampling() const
{
	return SAMPLING_NOT_NEEDED;
}