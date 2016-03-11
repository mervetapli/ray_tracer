#include <rt/materials/fuzzymirror.h>
#include <rt/solids/disc.h>

using namespace rt;

//ctor
FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle)
	: eta_(eta)
	, kappa_(kappa)
	, fuzzy_angle_(fuzzyangle)
{}

//getReflectance
RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
{
	return RGBColor::rep(0.0f);
}

//getEmission
RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
{
	return RGBColor::rep(0.0f);
}

//getSampleReflectance
Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
{
	SampleReflectance reflection;
	Vector n = normal;

	float cosine = dot(outDir, n);
	if (cosine<0.0f)
	{
		cosine = -cosine;
		n = -normal;
	}

	Disc temp(texPoint + (-outDir + 2 * cosine*n).normalize(), (-outDir + 2 * cosine*n).normalize(), tan(fuzzy_angle_), nullptr, nullptr);
	reflection.direction = temp.sample() - texPoint;

	float s = eta_*eta_ + kappa_*kappa_;
	float rparall = (s * cosine*cosine - 2.0f * eta_ * cosine + 1) / (s * cosine*cosine + 2.0f * eta_ * cosine + 1);
	float rperp = (s - 2.0f * eta_ * cosine + cosine*cosine) / (s + 2.0f * eta_ * cosine + cosine*cosine);

	reflection.reflectance = RGBColor::rep(1.0f) * (rparall + rperp) / 2.0f;

	return reflection;
}