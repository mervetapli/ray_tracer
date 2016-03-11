#include <rt/groups/simplegroup.h>
#include <rt/intersection.h>
#include <rt/bbox.h>

using namespace rt;

//getBounds
//CURRENTLY NOT IMPLEMENTED
BBox SimpleGroup::getBounds() const
{
	int size = primitives_.size();
	BBox bbox = BBox::empty();

	for (int i = 0; i < size; ++i)
	{
		bbox.extend(primitives_[i]->getBounds());
	}

	return bbox;
}

//intersect
Intersection SimpleGroup::intersect(const Ray& ray, float previousBestDistance) const
{
	int size = primitives_.size();
	Intersection nearest = Intersection::failure();
	Intersection temp;

	//nearest intersection is initially a failure.
	//if there is no intersection between a surface and a ray in a distance
	//less than previousBestDistance, it will be reported as no intersection.
	nearest.distance_ = previousBestDistance;

	for (int i = 0; i < size; ++i)
	{
		if (temp = primitives_[i]->intersect(ray, previousBestDistance))
		{
			//second boolean expression checks whether the distance is greater than zero.
			//this is because we do not want to project anything behind the camera.
			//when distance is negative, it means that the intersected object is
			//behind the camera.
			if (temp.distance_ < nearest.distance_ && temp.distance_ > 0)
			{
				nearest = temp;
			}
		}
	}

	return nearest;
}

//rebuildIndex
//CURRENTLY NOT IMPLEMENTED
void SimpleGroup::rebuildIndex()
{}

//add
void SimpleGroup::add(Primitive* p)
{
	primitives_.push_back(p);
}

//setMaterial
void SimpleGroup::setMaterial(Material* m)
{
	int size = primitives_.size();

	for (int i = 0; i < size; ++i)
	{
		primitives_[i]->setMaterial(m);
	}
}

//setCoordMapper
//CURRENTLY NOT IMPLEMENTED
void SimpleGroup::setCoordMapper(CoordMapper* cm)
{}