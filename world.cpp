#include <rt/coordmappers/world.h>
#include <core/float4.h>

using namespace rt;

//ctor
WorldMapper::WorldMapper()
	: scale_(Vector::rep(1.0f))
{}

//ctor
WorldMapper::WorldMapper(const Float4& scale)
	: scale_(scale)
{}

//ctor
Point WorldMapper::getCoords(const Intersection& hit) const
{
	Float4 s = Float4(scale_);
	s[3] = 1.0f;

	return Point(Float4(hit.hitPoint()) * s);
}