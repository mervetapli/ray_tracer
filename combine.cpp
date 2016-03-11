#include <rt/materials/combine.h>
#include <core/color.h>

using namespace rt;

//ctor
CombineMaterial::CombineMaterial()
	: sampling_(SAMPLING_NOT_NEEDED)
{}

//add
void CombineMaterial::add(Material* material, float weight)
{
	material_list_.push_back(std::make_pair(material, weight));

	if (material->useSampling() != Material::Sampling::SAMPLING_NOT_NEEDED)
	{
		sampling_ = SAMPLING_SECONDARY;
	}
}

//getReflectance
RGBColor CombineMaterial::getReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir, const Vector& in_dir) const
{
	int size = material_list_.size();
	RGBColor color(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < size; ++i)
	{
		color = color + material_list_[i].second*material_list_[i].first->getReflectance(tex_point, normal, out_dir, in_dir);
	}

	return color;
}

//getEmission
RGBColor CombineMaterial::getEmission(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	int size = material_list_.size();
	RGBColor color(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < size; ++i)
	{
		color = color + material_list_[i].second*material_list_[i].first->getEmission(tex_point, normal, out_dir);
	}

	return color;
}

//getSampleReflectance
Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& tex_point, const Vector& normal, const Vector& out_dir) const
{
	int size = material_list_.size();

	for (int i = 0; i < size; ++i)
	{
		if (material_list_[i].first->useSampling() != Material::Sampling::SAMPLING_NOT_NEEDED)
		{
			auto temp = material_list_[i].first->getSampleReflectance(tex_point, normal, out_dir);

			return SampleReflectance(temp.direction, temp.reflectance * material_list_[i].second);
		}
	}
}

//useSampling
Material::Sampling CombineMaterial::useSampling() const
{
	return sampling_;
}