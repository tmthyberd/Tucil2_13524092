// Will contain the AABB (axis-aligned bounding box) check where see if max min x y z is in the other max min xyz voxel cube
// If inside the AABB, then check the actual triangle overlap with triangle-box intersection

// For more efficiency, later on just keep a list of the triangles in a subarea that is the same with the voxel cube that is just divided to avoid needing to
// check the voxel cube against all the existing triangle faces.

#pragma once

#include "../struct/octree.hpp"
#include "../input/objLoader.hpp"
using namespace std;

NodeState overlapChecker(const Mesh &mesh, const OctreeNode &node);
bool aabbCheck(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, const OctreeNode &node);
bool triangleBoxIntersectionCheck(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, const OctreeNode &node);