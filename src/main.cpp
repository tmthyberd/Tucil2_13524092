#include <iostream>
#include <string>
#include <chrono>

#include "input/objLoader.hpp"
#include "struct/octree.hpp"
#include "algorithms/overlap_checker.hpp"
#include "algorithms/voxel_subdivider.hpp"
#include "output/objOutput.hpp"

using namespace std;

int main()
{
    string inputName;
    string outputName;
    int maxDepth;

    cout << "Input OBJ filename (inside test folder): ";
    cin >> inputName;

    cout << "Max octree depth: ";
    cin >> maxDepth;

    cout << "Output OBJ filename (inside output folder): ";
    cin >> outputName;

    string inputPath = "../test/" + inputName;
    string outputPath = "../output/" + outputName;

    Mesh mesh;
    if (!loadOBJ(inputPath, mesh))
    {
        cerr << "Failed to load OBJ file: " << inputPath << "\n";
        return 1;
    }

    CubeInfo cubeInfo = computeCube(mesh, 0.001f);

    Octree octree(cubeInfo.cx, cubeInfo.cy, cubeInfo.cz, cubeInfo.halfSize, maxDepth);

    // Timer voxelization
    auto startTime = chrono::high_resolution_clock::now();

    voxelSubdivider(*octree.root, octree.maxDepth, mesh);

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = endTime - startTime;

    if (!voxelObjOutput(octree, outputPath))
    {
        cerr << "Failed to write OBJ output: " << outputPath << "\n";
        return 1;
    }

    OctreeStats stats = collectOctreeStats(octree);

    int vertexCount = stats.voxelCount * 8;
    int faceCount = stats.voxelCount * 12;

    cout << "Voxel count    : " << stats.voxelCount << "\n";
    cout << "Vertex count   : " << vertexCount << "\n";
    cout << "Face count     : " << faceCount << "\n";

    cout << "\nOctree node per depth:" << "\n";
    for (int i = 0; i <= maxDepth; i++)
    {
        cout << "  " << i << " : " << stats.nodesPerDepth[i] << "\n";
    }

    cout << "\nPruned node per depth (EMPTY, tidak perlu ditelusuri):" << "\n";
    for (int i = 0; i <= maxDepth; i++)
    {
        cout << "  " << i << " : " << stats.prunedPerDepth[i] << "\n";
    }

    cout << "\nOctree depth   : " << maxDepth << "\n";
    cout << "Execution time : " << elapsed.count() << " ms" << "\n";
    cout << "Output path    : " << outputPath << "\n";

    return 0;
}