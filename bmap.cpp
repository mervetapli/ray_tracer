#include <rt/primmod/bmap.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <rt/solids/triangle.h>
#include <core/interpolate.h>
#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/vector.h>
//#include <rt/solids/striangle.h>

using namespace rt;

//ctor
BumpMapper::BumpMapper(Triangle* base, Texture* bump_map, const Point& v1, const Point& v2, const Point& v3, float vscale)
	: base_(base)
	, bump_map_(bump_map)
	, v1_(v1)
	, v2_(v2)
	, v3_(v3)
	, scale_(vscale)
{
	if (std::abs((v1 - v2).x_) > 0)
		u_ = (base->v1_ - base->v2_) / (v1 - v2).x_;
	else
		u_ = (base->v1_ - base->v3_) / (v1 - v3).x_;


	if (std::abs((v1 - v2).y_) > 0)
		v_ = (base->v1_ - base->v2_) / (v1 - v2).y_;
	else
		v_ = (base->v1_ - base->v3_) / (v1 - v3).y_;
}

//getBounds
BBox BumpMapper::getBounds() const
{
	return base_->getBounds();
}

//intersect
Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const
{
	Intersection intersection = base_->intersect(ray, previousBestDistance);
	
	if (intersection)
	{
		Point barycentric = intersection.local();
		Point hitpoint_uv = lerpbar(v1_, v2_, v3_, barycentric.x_, barycentric.y_);

		auto dx = bump_map_->getColorDX(hitpoint_uv);
		auto dy = bump_map_->getColorDY(hitpoint_uv);

		auto normal = (intersection.normal() + u_*dx.r + v_*dy.r).normalize();

		return Intersection(intersection.distance_, ray, intersection.solid_, normal, intersection.local(), true);
	}

	return Intersection::failure();
}

//setMaterial
//CURRENTLY NOT IMPLEMENTED
void BumpMapper::setMaterial(Material* m)
{}

//setCoordMapper
//CURRENTLY NOT IMPLEMENTED
void BumpMapper::setCoordMapper(CoordMapper* cm)
{}