#include <rt/cameras/perspective.h>
#include <cmath>
#include <rt/ray.h>
#include <core/scalar.h>

using namespace rt;

//ctor
PerspectiveCamera::PerspectiveCamera(
	const Point& center,
	const Vector& forward,
	const Vector& up,
	float verticalOpeningAngle,
	float horizonalOpeningAngle
	)
	: position_(center)
	, up_(up.normalize())
	, focal_(forward)
{
	right_ = cross(focal_, up_).normalize();
	up_ = cross(focal_, -right_).normalize();
	span_x_ = right_*focal_.length()*std::tan(horizonalOpeningAngle / 2);
	span_y_ = -up_*focal_.length()*std::tan(verticalOpeningAngle / 2);
}

//getPrimaryRay
Ray PerspectiveCamera::getPrimaryRay(float x, float y) const
{
	return Ray(position_, (focal_ + x*span_x_ + y*span_y_).normalize());
}