#ifndef CG1RAYTRACER_LIGHTS_LIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_LIGHT_HEADER

#include <core/vector.h>

namespace rt
{
	class RGBColor;
	class Point;

	struct LightHit
	{
		Vector direction;  //the direction where the light is coming FROM at the given point
		float distance;    //the distance that has to be checked, along the direction, for occlusion

		LightHit(const Vector& p_direction = Vector(), float p_distance = float())
			: direction(p_direction)
			, distance(p_distance)
		{}
	};

	class Light
	{
	public:
		virtual LightHit getLightHit(const Point& p) const = 0;
		virtual RGBColor getIntensity(const LightHit& irr) const = 0;
	};
}

#endif