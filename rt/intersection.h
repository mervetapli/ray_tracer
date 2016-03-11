#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt
{
	class Solid;

	class Intersection
	{
	public:
		float distance_;
		Ray ray_;
		const Solid* solid_;
		bool b_intersection_;

	public:
		Intersection();
		Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local, bool b_intersection = false);
		
		static Intersection failure();

		Point hitPoint() const;
		Vector normal() const;
		Point local() const;

		operator bool();

	private:
		Vector normal_;
		Point local_;
	};
}

#endif