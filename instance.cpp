#include <rt/primmod/instance.h>
#include <cmath>
#include <rt/bbox.h>
#include <rt/intersection.h>

using namespace rt;

//Instance
Instance::Instance(Primitive* content)
	: content_(content)
	, transformation_(Matrix::identity())
{}

//content
Primitive* Instance::content()
{
	return content_;
}

//reset
void Instance::reset()
{
	transformation_ = Matrix::identity();
}

//translate
void Instance::translate(const Vector& t)
{
	Matrix translation = Matrix::identity();
	translation[0][3] = t.x_;
	translation[1][3] = t.y_;
	translation[2][3] = t.z_;

	transformation_ = product(translation, transformation_);
}

//rotate
void Instance::rotate(const Vector& axis, float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float k = 1 - cos;

	Matrix rotation(
		Float4(cos + axis.x_*axis.x_*k, axis.x_*axis.y_*k - axis.z_*sin, axis.x_*axis.z_*k + axis.y_*sin, 0.0f),
		Float4(axis.y_*axis.x_*k + axis.z_*sin, cos + axis.y_*axis.y_*k, axis.y_*axis.z_*k - axis.x_*sin, 0.0f),
		Float4(axis.z_*axis.x_*k - axis.y_*sin, axis.z_*axis.y_*k + axis.x_*sin, cos + axis.z_*axis.z_*k, 0.0f),
		Float4(0.0f, 0.0f, 0.0f, 1.0f));

	transformation_ = product(rotation, transformation_);
}

//scale
void Instance::scale(float scale)
{
	Matrix scale_matrix = Matrix::identity()*scale;
	scale_matrix[3][3] = 1.0f;

	transformation_ = product(scale_matrix, transformation_);
}

//scale
void Instance::scale(const Vector& scale)
{
	Matrix scale_matrix = Matrix::identity();
	scale_matrix[0][0] = scale.x_;
	scale_matrix[1][1] = scale.y_;
	scale_matrix[2][2] = scale.z_;

	transformation_ = product(scale_matrix, transformation_);
}

//getBounds
BBox Instance::getBounds() const
{
	BBox bbox = content_->getBounds();

	Point min = transformation_*bbox.min_;
	Point max = transformation_*bbox.max_;

	bbox.min_ = rt::min(min, max);
	bbox.max_ = rt::max(min, max);
		
	return bbox;
}

//intersect
Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const
{
	Point ray_origin = ray.o_;
	Vector ray_vector = ray.d_;
	Matrix inverse_transformation = transformation_.invert();
	ray_origin = inverse_transformation*ray_origin;
	ray_vector = (inverse_transformation*ray_vector).normalize();

	Intersection intersection = content_->intersect(Ray(ray_origin, ray_vector), previousBestDistance);
	if (!intersection.b_intersection_)
	{
		return Intersection::failure();
	}

	//notice that transforming back the normal vector is different
	Vector normal = (transformation_.transpose().invert()*intersection.normal()).normalize();
	Point hit_point = transformation_*intersection.hitPoint();
	
	//local coordinate will not be transformed!
	return Intersection((hit_point - ray.o_).length(), ray, intersection.solid_, normal, intersection.local(), true);
}

//setMaterial
//CURRENTLY NOT IMPLEMENTED
void Instance::setMaterial(Material* m)
{}

//setCoordMapper
//CURRENTLY NOT IMPLEMENTED
void Instance::setCoordMapper(CoordMapper* cm)
{}