#ifndef CG1RAYTRACER_VECTOR_HEADER
#define CG1RAYTRACER_VECTOR_HEADER

#include <core/macros.h>

namespace rt
{
	class Float4;
	class Point;

	class ALIGN(16) Vector
	{
	public:
		float x_, y_, z_;

	public:
		Vector() {}
		Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}
		explicit Vector(const Float4& f4);

		static Vector rep(float v) { return Vector(v, v, v); }

		Vector operator+(const Vector& v) const;
		Vector operator-(const Vector& v) const;

		Vector normalize() const;

		Vector operator-() const;

		float lensqr() const;
		float length() const;

		bool operator==(const Vector& v) const;
		bool operator!=(const Vector& v) const;
	};

	Vector operator*(float scalar, const Vector& v);
	Vector operator*(const Vector& v, float scalar);
	Vector operator/(const Vector& v, float scalar);

	Vector cross(const Vector& u, const Vector& v);
	float dot(const Vector& u, const Vector& v);

	Vector min(const Vector& u, const Vector& v);
	Vector max(const Vector& u, const Vector& v);

	Point operator+(const Point& p, const Vector& v);
	Point operator+(const Vector& v, const Point& p);
	Point operator-(const Point& p, const Vector& v);
	Point operator*(const Float4& scale, const Point& p);
}

#endif