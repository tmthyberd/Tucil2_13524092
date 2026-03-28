// Will take the list of voxel cube middle points (octree)

// Outputs using normal faces (12 normal faces for 1 voxel cube)

// Boundary voxel

#pragma once

#include <string>
#include <vector>
#include "../struct/octree.hpp"

struct OctreeStats
{
    int voxelCount;                    // Jumlah voxel BOUNDARY di maxDepth
    std::vector<int> nodesPerDepth;    // Banyaknya node yang terbentuk per depth
    std::vector<int> prunedPerDepth;   // Banyaknya node EMPTY (tidak perlu ditelusuri) per depth
};

bool voxelObjOutput(const Octree &octree, const std::string &filename);
OctreeStats collectOctreeStats(const Octree &octree);