#include <rt/groups/bvh.h>
#include <stack>
#include <rt/intersection.h>

using namespace rt;

//ctor
BVH::BVH()
{}

//dtor
BVH::~BVH()
{}

//getBounds
BBox BVH::getBounds() const
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
Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const
{
	std::stack<Node*> st;
	st.push(root_);

	Node *node;
	Intersection final_intersection = Intersection::failure();
	final_intersection.distance_ = previousBestDistance;
	while (!st.empty())
	{
		node = st.top();
		st.pop();

		if (!isLeaf(node))
		{
			auto pair = node->right->bbox.intersect(ray);
			if (pair.first < pair.second)
			{
				st.push(node->right);
			}

			pair = node->left->bbox.intersect(ray);
			if (pair.first < pair.second)
			{
				st.push(node->left);
			}
		}

		else
		{
			//nearest intersection is initially a failure.
			//if there is no intersection between a surface and a ray in a distance
			//less than previousBestDistance, it will be reported as no intersection.
			int size = node->primitive_indices.size();
			Intersection nearest = Intersection::failure();
			nearest.distance_ = previousBestDistance;
			Intersection temp;

			for (int i = 0; i < size; ++i)
			{
				if (temp = primitives_[node->primitive_indices[i]]->intersect(ray, previousBestDistance))
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

			if (nearest.b_intersection_ && nearest.distance_ < final_intersection.distance_)
			{
				final_intersection = nearest;
			}
		}
	}

	return final_intersection;
}

//rebuildIndex
void BVH::rebuildIndex()
{
	int size = primitives_.size();

	bboxes_.resize(size);
	centroids_.resize(size);
	root_ = new Node;
	root_->bbox = getBounds();
	root_->primitive_indices.resize(size);

	for (int i = 0; i < size; ++i)
	{
		bboxes_[i] = primitives_[i]->getBounds();
		centroids_[i] = bboxes_[i].min_ + bboxes_[i].diagonal() / 2;
		root_->primitive_indices[i] = i;
	}

	buildTree();
}

//add
void BVH::add(Primitive* p)
{
	primitives_.push_back(p);
}

//setMaterial
void BVH::setMaterial(Material* m)
{
	int size = primitives_.size();

	for (int i = 0; i < size; ++i)
	{
		primitives_[i]->setMaterial(m);
	}
}

//setCoordMapper
//CURRENTLY NOT IMPLEMENTED
void BVH::setCoordMapper(CoordMapper* cm)
{}

//buildTree
void BVH::buildTree()
{
	const int SAMPLE_SIZE = 15;
	BBox bbox_partition_left[SAMPLE_SIZE];
	BBox bbox_partition_right[SAMPLE_SIZE];
	uint primitives_partition[SAMPLE_SIZE] = { 0 };
	float cost[SAMPLE_SIZE];
	uint cheap_index;
	float cheap_cost;

	std::stack<Node*> st;
	st.push(root_);

	Node *node;
	while (!st.empty())
	{
		node = st.top();
		st.pop();

		int size = node->primitive_indices.size();

		//partition it
		if (size > 30)
		{
			node->left = new Node;
			node->right = new Node;

			node->left->bbox = BBox::empty();
			node->right->bbox = BBox::empty();

			//decide the split axis and children's bounding boxes
			auto &centroids = centroids_;
			auto diagonal = node->bbox.diagonal();

			//split along the x-axis
			if (diagonal.x_ > diagonal.y_ && diagonal.x_ > diagonal.z_)
			{
				std::sort(node->primitive_indices.begin(), node->primitive_indices.end(), [&centroids](uint a, uint b)
				{ return centroids[a].x_ < centroids[b].x_; });

				//if size is greater than 20, use SAH(surface area heuristic).
				if (size > 20)
				{
					//equally distant sample points.
					float equal_dist = (node->bbox.max_.x_ - node->bbox.min_.x_) / (SAMPLE_SIZE + 1);

					//compute probable bounding boxes and number of primitives they contain.
					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						for (int j = 0; j < size; ++j)
						{
							if (centroids_[node->primitive_indices[j]].x_ <= (node->bbox.min_.x_ + equal_dist*(i + 1)))
							{
								bbox_partition_left[i].extend(bboxes_[node->primitive_indices[j]]);
								++primitives_partition[i];
							}

							else
							{
								bbox_partition_right[i].extend(bboxes_[node->primitive_indices[j]]);
							}
						}

						//compute cost for each case.
						cost[i] = 0.125f + (bbox_partition_left[i].getArea()*primitives_partition[i]
							+ bbox_partition_right[i].getArea()*(node->primitive_indices.size() - primitives_partition[i])) / node->bbox.getArea();

						if (primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						if (cost[i] < cheap_cost && primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					//split the volume
					for (int i = 0; i < size; ++i)
					{
						if (centroids_[node->primitive_indices[i]].x_ <= (node->bbox.min_.x_ + equal_dist*(cheap_index + 1)))
						{
							node->left->primitive_indices.push_back(node->primitive_indices[i]);
						}

						else
						{
							node->right->primitive_indices.push_back(node->primitive_indices[i]);
						}
					}

					node->left->bbox = bbox_partition_left[cheap_index];
					node->right->bbox = bbox_partition_right[cheap_index];
				}

				//if size is less than 18, use median splitting.
				else
				{
					int i = 0;
					for (; i < size / 2; ++i)
					{
						node->left->primitive_indices.push_back(node->primitive_indices[i]);
						node->left->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}

					for (; i < size; ++i)
					{
						node->right->primitive_indices.push_back(node->primitive_indices[i]);
						node->right->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}
				}
			}

			//
			//split along the y-axis
			else if (diagonal.y_ > diagonal.z_)
			{
				std::sort(node->primitive_indices.begin(), node->primitive_indices.end(), [&centroids](uint a, uint b)
				{ return centroids[a].y_ < centroids[b].y_; });

				//if size is greater than 18, use SAH(surface area heuristic).
				if (size > 18)
				{
					//equally distant sample points.
					float equal_dist = (node->bbox.max_.y_ - node->bbox.min_.y_) / (SAMPLE_SIZE + 1);

					//compute probable bounding boxes and number of primitives they contain.
					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						for (int j = 0; j < size; ++j)
						{
							if (centroids_[node->primitive_indices[j]].y_ <= (node->bbox.min_.y_ + equal_dist*(i + 1)))
							{
								bbox_partition_left[i].extend(bboxes_[node->primitive_indices[j]]);
								++primitives_partition[i];
							}

							else
							{
								bbox_partition_right[i].extend(bboxes_[node->primitive_indices[j]]);
							}
						}

						//compute cost for each case.
						cost[i] = 0.125f + (bbox_partition_left[i].getArea()*primitives_partition[i]
							+ bbox_partition_right[i].getArea()*(node->primitive_indices.size() - primitives_partition[i])) / node->bbox.getArea();

						if (primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					//find the cheapest cost
					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						if (cost[i] < cheap_cost && primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					//split the volume at where that gives the cheapest cost
					for (int i = 0; i < size; ++i)
					{
						if (centroids_[node->primitive_indices[i]].y_ <= (node->bbox.min_.y_ + equal_dist*(cheap_index + 1)))
						{
							node->left->primitive_indices.push_back(node->primitive_indices[i]);
						}

						else
						{
							node->right->primitive_indices.push_back(node->primitive_indices[i]);
						}
					}

					node->left->bbox = bbox_partition_left[cheap_index];
					node->right->bbox = bbox_partition_right[cheap_index];
				}

				//if size is less than 18, use median splitting.
				else
				{
					int i = 0;
					for (; i < size / 2; ++i)
					{
						node->left->primitive_indices.push_back(node->primitive_indices[i]);
						node->left->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}

					for (; i < size; ++i)
					{
						node->right->primitive_indices.push_back(node->primitive_indices[i]);
						node->right->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}
				}
			}

			//
			//split along the z-axis
			else
			{
				std::sort(node->primitive_indices.begin(), node->primitive_indices.end(), [&centroids](uint a, uint b)
				{ return centroids[a].z_ < centroids[b].z_; });

				//if size is greater than 18, use SAH(surface area heuristic).
				if (size > 18)
				{
					//equally distant sample points.
					float equal_dist = (node->bbox.max_.z_ - node->bbox.min_.z_) / (SAMPLE_SIZE + 1);

					//compute probable bounding boxes and number of primitives they contain.
					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						for (int j = 0; j < size; ++j)
						{
							if (centroids_[node->primitive_indices[j]].z_ <= (node->bbox.min_.z_ + equal_dist*(i + 1)))
							{
								bbox_partition_left[i].extend(bboxes_[node->primitive_indices[j]]);
								++primitives_partition[i];
							}

							else
							{
								bbox_partition_right[i].extend(bboxes_[node->primitive_indices[j]]);
							}
						}

						//compute cost for each case.
						cost[i] = 0.125f + (bbox_partition_left[i].getArea()*primitives_partition[i]
							+ bbox_partition_right[i].getArea()*(node->primitive_indices.size() - primitives_partition[i])) / node->bbox.getArea();

						if (primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					for (int i = 0; i < SAMPLE_SIZE; ++i)
					{
						if (cost[i] < cheap_cost && primitives_partition[i] > 0 && primitives_partition[i] < node->primitive_indices.size())
						{
							cheap_cost = cost[i];
							cheap_index = i;
						}
					}

					//split the volume
					for (int i = 0; i < size; ++i)
					{
						if (centroids_[node->primitive_indices[i]].z_ <= (node->bbox.min_.z_ + equal_dist*(cheap_index + 1)))
						{
							node->left->primitive_indices.push_back(node->primitive_indices[i]);
						}

						else
						{
							node->right->primitive_indices.push_back(node->primitive_indices[i]);
						}
					}

					node->left->bbox = bbox_partition_left[cheap_index];
					node->right->bbox = bbox_partition_right[cheap_index];
				}

				//if size is less than 18, use median splitting.
				else
				{
					int i = 0;
					for (; i < size / 2; ++i)
					{
						node->left->primitive_indices.push_back(node->primitive_indices[i]);
						node->left->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}

					for (; i < size; ++i)
					{
						node->right->primitive_indices.push_back(node->primitive_indices[i]);
						node->right->bbox.extend(bboxes_[node->primitive_indices[i]]);
					}
				}
			}

			//reset
			for (int i = 0; i < SAMPLE_SIZE; ++i)
			{
				bbox_partition_left[i] = BBox::empty();
				bbox_partition_right[i] = BBox::empty();
				primitives_partition[i] = 0;
			}

			//clear it now because it is not a leaf
			node->primitive_indices.clear();

			//recurse
			st.push(node->right);
			st.push(node->left);
		}
	}
}

//isLeaf
inline bool BVH::isLeaf(Node* node) const
{
	return node->left == nullptr && node->right == nullptr;
}