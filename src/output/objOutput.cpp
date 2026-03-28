#include "objOutput.hpp"

#include <fstream>
#include <iostream>

using namespace std;

bool isLeaf(const OctreeNode *node)
{
    for (int i = 0; i < 8; i++)
    {
        if (node->children[i] != nullptr)
        {
            return false;
        }
    }
    return true;
}

void writeCube(ofstream &file, const OctreeNode *node, int &vertexOffset)
{
    float h = node->halfSize;

    float minX = node->cx - h;
    float maxX = node->cx + h;

    float minY = node->cy - h;
    float maxY = node->cy + h;

    float minZ = node->cz - h;
    float maxZ = node->cz + h;

    // 8 vertices
    file << "v " << minX << " " << minY << " " << minZ << "\n"; // 1
    file << "v " << maxX << " " << minY << " " << minZ << "\n"; // 2
    file << "v " << maxX << " " << maxY << " " << minZ << "\n"; // 3
    file << "v " << minX << " " << maxY << " " << minZ << "\n"; // 4
    file << "v " << minX << " " << minY << " " << maxZ << "\n"; // 5
    file << "v " << maxX << " " << minY << " " << maxZ << "\n"; // 6
    file << "v " << maxX << " " << maxY << " " << maxZ << "\n"; // 7
    file << "v " << minX << " " << maxY << " " << maxZ << "\n"; // 8

    int v1 = vertexOffset + 1;
    int v2 = vertexOffset + 2;
    int v3 = vertexOffset + 3;
    int v4 = vertexOffset + 4;
    int v5 = vertexOffset + 5;
    int v6 = vertexOffset + 6;
    int v7 = vertexOffset + 7;
    int v8 = vertexOffset + 8;

    // 12 triangle faces

    // Bottom face (z = minZ)
    file << "f " << v1 << " " << v2 << " " << v3 << "\n";
    file << "f " << v1 << " " << v3 << " " << v4 << "\n";

    // Top face (z = maxZ)
    file << "f " << v5 << " " << v7 << " " << v6 << "\n";
    file << "f " << v5 << " " << v8 << " " << v7 << "\n";

    // Front face (y = minY)
    file << "f " << v1 << " " << v6 << " " << v2 << "\n";
    file << "f " << v1 << " " << v5 << " " << v6 << "\n";

    // Back face (y = maxY)
    file << "f " << v4 << " " << v3 << " " << v7 << "\n";
    file << "f " << v4 << " " << v7 << " " << v8 << "\n";

    // Left face (x = minX)
    file << "f " << v1 << " " << v4 << " " << v8 << "\n";
    file << "f " << v1 << " " << v8 << " " << v5 << "\n";

    // Right face (x = maxX)
    file << "f " << v2 << " " << v6 << " " << v7 << "\n";
    file << "f " << v2 << " " << v7 << " " << v3 << "\n";

    vertexOffset += 8;
}

void traverseAndWrite(ofstream &file, const OctreeNode *node, int maxDepth, int &vertexOffset)
{
    if (node == nullptr)
    {
        return;
    }

    if (isLeaf(node))
    {
        if (node->state == BOUNDARY && node->depth == maxDepth)
        {
            writeCube(file, node, vertexOffset);
        }
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        traverseAndWrite(file, node->children[i], maxDepth, vertexOffset);
    }
}

void traverseStats(const OctreeNode *node, int maxDepth, OctreeStats &stats)
{
    if (node == nullptr)
        return;

    stats.nodesPerDepth[node->depth]++;

    if (node->state == EMPTY)
    {
        stats.prunedPerDepth[node->depth]++;
        return;
    }

    if (isLeaf(node))
    {
        if (node->state == BOUNDARY && node->depth == maxDepth)
            stats.voxelCount++;
        return;
    }

    for (int i = 0; i < 8; i++)
        traverseStats(node->children[i], maxDepth, stats);
}

OctreeStats collectOctreeStats(const Octree &octree)
{
    OctreeStats stats;
    stats.voxelCount = 0;
    stats.nodesPerDepth.resize(octree.maxDepth + 1, 0);
    stats.prunedPerDepth.resize(octree.maxDepth + 1, 0);

    traverseStats(octree.root, octree.maxDepth, stats);

    return stats;
}

bool voxelObjOutput(const Octree &octree, const string &filename)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cerr << "Failed to open output OBJ file: " << filename << "\n";
        return false;
    }

    int vertexOffset = 0;

    traverseAndWrite(file, octree.root, octree.maxDepth, vertexOffset);

    file.close();
    return true;
}