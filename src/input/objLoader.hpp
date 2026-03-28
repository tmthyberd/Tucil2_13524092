#pragma once

#include <string>
#include <vector>

using namespace std;

struct Vec3
{
    float x, y, z;
};

struct Face
{
    int v1, v2, v3;
};

struct Mesh
{
    vector<Vec3> vertices;
    vector<Face> faces;

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

struct CubeInfo
{
    float cx, cy, cz;
    float halfSize;
};

bool loadOBJ(const string &filename, Mesh &mesh);
CubeInfo computeCube(const Mesh &mesh, float padding = 0.001f);