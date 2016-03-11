#include <rt/materials/mirror.h>
#include <cmath>
#include <core/scalar.h>

using namespace rt;

//ctor
MirrorMaterial::MirrorMaterial(float eta, float kappa)
	: eta_(eta)
	, kappa_(kappa)
{}

//getReflectance
RGBColor MirrorMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	return RGBColor::rep(0.0f);
}

//getEmission
RGBColor MirrorMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	return RGBColor::rep(0.0f);
}

//getSampleReflectance
Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	SampleReflectance reflection;
	Vector n = normal;
	float cosine = dot(out_dir, n);
	if (cosine < 0.0f)
	{
		cosine = -cosine;
		n = -normal;
	}
	reflection.direction = (-out_dir + 2 * cosine*n).normalize();

	float s = sqr(eta_) + sqr(kappa_);
	float rparall = (s * sqr(cosine) - 2.0f * eta_ * cosine + 1) / (s * sqr(cosine) + 2.0f * eta_ * cosine + 1);
	float rperp = (s - 2.0f * eta_ * cosine + sqr(cosine)) / (s + 2.0f * eta_ * cosine + sqr(cosine));

	reflection.reflectance = RGBColor::rep(1.0f) * (rparall + rperp) / 2.0f;

	return reflection;
}

//useSampling
Material::Sampling MirrorMaterial::useSampling() const
{
	return SAMPLING_ALL;
}