#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt
{
	class BVH : public Group
	{
	public:
		struct Node
		{
			BBox bbox;
			Node* left;
			Node* right;

			std::vector<uint> primitive_indices;

			Node()
				: left(nullptr)
				, right(nullptr)
			{}
		};

	public:
		BVH();
		virtual ~BVH();

		virtual BBox getBounds() const;
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
		virtual void rebuildIndex();
		virtual void add(Primitive* p);
		virtual void setMaterial(Material* m);
		virtual void setCoordMapper(CoordMapper* cm);

	private:
		Primitives primitives_;
		std::vector<BBox> bboxes_;
		std::vector<Point> centroids_;
		Node* root_;

	private:
		void buildTree();
		bool isLeaf(Node* node) const;
	};
}

#endif