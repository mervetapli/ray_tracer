#include <rt/cameras/dofperspective.h>
#include <cmath>
#include <rt/ray.h>
#include <core/scalar.h>
#include <core/random.h>

using namespace rt;

//ctor
DOFPerspectiveCamera::DOFPerspectiveCamera(
	const Point& center,
	const Vector& forward,
	const Vector& up,
	float verticalOpeningAngle,
	float horizonalOpeningAngle,
	float focalDistance,
	float apertureRadius
	)
	: position_(center)
	, up_(up.normalize())
	, focal_(forward)
	, focal_distance_(focalDistance)
	, aperture_radius_(apertureRadius)
{
	right_ = cross(focal_, up_).normalize();
	up_ = cross(focal_, -right_).normalize();
	span_x_ = right_*focal_.length()*std::tan(horizonalOpeningAngle / 2);
	span_y_ = -up_*focal_.length()*std::tan(verticalOpeningAngle / 2);
}

//getPrimaryRay
Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const
{
	Ray ray;
	auto focal_point = position_ + (focal_ + x*span_x_ + y*span_y_).normalize()*focal_distance_;

	float radius = random(0.0f, aperture_radius_);
	float angle = 2 * pi * random();
	float u = radius * cos(angle);
	float v = radius * sin(angle);

	ray.o_ = position_ + u*right_ + v*up_;
	ray.d_ = (focal_point - ray.o_).normalize();

	return ray;
}