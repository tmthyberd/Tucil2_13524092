#pragma once

// Will contain the algorithm to divide the cubes into 8^n where n is the depth (it will be inputted in the main program)
#include "../struct/octree.hpp"
#include "../input/objLoader.hpp"
#include "overlap_checker.hpp"

void voxelSubdivider(OctreeNode &root, int maxDepth, const Mesh &mesh);