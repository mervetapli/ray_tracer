#include <rt/textures/perlin.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/assert.h>

using namespace rt;

namespace rt
{
	namespace
	{
		//returns a value in range -1 to 1
		float noise(int x, int y, int z)
		{
			int n = x + y * 57 + z * 997;
			n = (n << 13) ^ n;
			return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
		}
	}
}

//ctor
PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
	: white_(white)
	, black_(black)
{}

//addOctave
void PerlinTexture::addOctave(float amplitude, float frequency)
{
	octave_list_.push_back(std::make_pair(amplitude, frequency));
}

//getColor
RGBColor PerlinTexture::getColor(const Point& coord)
{
	int size = octave_list_.size();

	float total_noise = 0.0f;
	Point point;
	for (int i = 0; i < size; ++i)
	{
		point = coord * octave_list_[i].second;

		int lu = floor(point.x_);
		int lv = floor(point.y_);
		int lw = floor(point.z_);

		float fu = point.x_ - lu;
		float fv = point.y_ - lv;
		float fw = point.z_ - lw;

		float v1 = (noise(lu, lv, lw) + 1) / 2;
		float v2 = (noise(lu + 1, lv, lw) + 1) / 2;
		float v3 = (noise(lu, lv + 1, lw) + 1) / 2;
		float v4 = (noise(lu + 1, lv + 1, lw) + 1) / 2;
		float v5 = (noise(lu, lv, lw + 1) + 1) / 2;
		float v6 = (noise(lu + 1, lv, lw + 1) + 1) / 2;
		float v7 = (noise(lu, lv + 1, lw + 1) + 1) / 2;
		float v8 = (noise(lu + 1, lv + 1, lw + 1) + 1) / 2;

		total_noise += abs(octave_list_[i].first * lerp3d(v1, v2, v3, v4, v5, v6, v7, v8, fu, fv, fw));
	}

	return lerp(black_, white_, total_noise);
}

//getColorDX
RGBColor PerlinTexture::getColorDX(const Point& coord)
{
	NOT_IMPLEMENTED;
}

//getColorDY
RGBColor PerlinTexture::getColorDY(const Point& coord)
{
	NOT_IMPLEMENTED;
}