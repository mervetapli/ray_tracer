#ifndef CG1RAYTRACER_LIGHTS_DIRECTIONAL_HEADER
#define CG1RAYTRACER_LIGHTS_DIRECTIONAL_HEADER

#include <rt/lights/light.h>
#include <core/color.h>

namespace rt
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() {}
		DirectionalLight(const Vector& direction, const RGBColor& intensity);
		virtual LightHit getLightHit(const Point& p) const;
		virtual RGBColor getIntensity(const LightHit& irr) const;

	private:
		Vector direction_;
		RGBColor intensity_;
	};
}

#endif