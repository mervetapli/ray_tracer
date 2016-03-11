#include <rt/materials/glass.h>
#include <cmath>
#include <core/random.h>

using namespace rt;

GlassMaterial::GlassMaterial(float eta)
	: eta_(eta)
{}


RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
{
	return RGBColor::rep(0.0f);
}

RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
{
	return RGBColor::rep(0.0f);
}

Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
{
	SampleReflectance reflection;

	float cos_i = dot(outDir.normalize(), normal.normalize());
	float cos = dot(outDir.normalize(), normal.normalize());

	float eta = eta_;

	Vector norm = normal;

	if (cos_i < 0)
	{
		norm = -normal;
		eta = 1.0f / eta_;
		cos_i = -cos_i;
	}

	if ((1.0f - (1.0f - cos_i * cos_i) / (eta * eta)) < 0.0f)
	{
		reflection.direction = 2 * dot(normal.normalize(), outDir.normalize()) * normal.normalize() - outDir.normalize();
		reflection.reflectance = RGBColor::rep(1.0f);
		return reflection;
	}
	
	float cos_t = sqrt(1.0f - (1.0f - (cos_i*cos_i)) / (eta*eta));

	float randNum = random();

	float rperp = (eta * cos - cos_t) / (eta * cos + cos_t);
	float rparall = (cos - eta * cos_t) / (cos + eta * cos_t);

	float ref_index = (rparall*rparall + rperp*rperp) / 2;
	float trans_index = 1 - ref_index;

	if (randNum > 0.5f) // reflection
	{
		reflection.direction = 2 * dot(normal.normalize(), outDir.normalize()) * normal - outDir;
		reflection.reflectance = RGBColor::rep(2* ref_index);
	}
	else // transmission
	{
		reflection.direction = -outDir / eta - (sqrtf(1.0f - (1.0f - cos_i * cos_i) / (eta * eta)) - cos_i / eta) * norm;
		reflection.reflectance = RGBColor::rep(2* trans_index);
	}

	return reflection;
}
