#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/scalar.h>
#include <core/point.h>
#include <core/vector.h>

namespace rt
{
	class Ray;

	class BBox
	{
	public:
		Point min_, max_;

	public:
		BBox(const Point& min = Point(FLT_MAX, FLT_MAX, FLT_MAX), const Point& max = Point(-FLT_MAX, -FLT_MAX, -FLT_MAX))
			: min_(min)
			, max_(max)
		{}

		static BBox empty();
		static BBox full();

		void extend(const Point& point);
		void extend(const BBox& bbox);

		Vector diagonal() const { return max_ - min_; }

		std::pair<float, float> intersect(const Ray& ray) const;

		bool isUnbound();

		float getArea() const;
	};
}

#endif