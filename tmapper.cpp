#include <rt/coordmappers/tmapper.h>
#include <core/float4.h>
#include <rt/intersection.h>
#include <core/interpolate.h>

using namespace rt;

//ctor
TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
	vertices_[0] = tv0;
	vertices_[1] = tv1;
	vertices_[2] = tv2;
}

//ctor
TriangleMapper::TriangleMapper(Point ntv[3])
{
	vertices_[0] = ntv[0];
	vertices_[1] = ntv[1];
	vertices_[2] = ntv[2];
}

//getCoords
Point TriangleMapper::getCoords(const Intersection& hit) const
{
	return Point(lerpbar(vertices_[0], vertices_[1], vertices_[2], hit.local().x_, hit.local().y_));
}