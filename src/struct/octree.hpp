#pragma once

enum NodeState
{
    EMPTY,
    BOUNDARY
};

struct OctreeNode
{
    float cx, cy, cz;
    float halfSize;
    int depth;

    NodeState state;

    OctreeNode *children[8];

    OctreeNode(float x, float y, float z, float h, int d)
        : cx(x), cy(y), cz(z), halfSize(h), depth(d), state(EMPTY)
    {
        for (int i = 0; i < 8; i++)
            children[i] = nullptr;
    }
};

struct Octree
{
    OctreeNode *root;
    int maxDepth;

    Octree(float x, float y, float z, float h, int md)
        : maxDepth(md)
    {
        root = new OctreeNode(x, y, z, h, 0);
        root->state = BOUNDARY; // Pasti root cube overlap
    }
};