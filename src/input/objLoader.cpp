#include "objLoader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

bool loadOBJ(const string &filename, Mesh &mesh)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Failed to open OBJ file: " << filename << '\n';
        return false;
    }

    mesh.vertices.clear();
    mesh.faces.clear();

    mesh.minX = mesh.minY = mesh.minZ = numeric_limits<float>::max();
    mesh.maxX = mesh.maxY = mesh.maxZ = numeric_limits<float>::lowest();

    string line;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            Vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;

            mesh.vertices.push_back(vertex);

            mesh.minX = min(mesh.minX, vertex.x);
            mesh.minY = min(mesh.minY, vertex.y);
            mesh.minZ = min(mesh.minZ, vertex.z);

            mesh.maxX = max(mesh.maxX, vertex.x);
            mesh.maxY = max(mesh.maxY, vertex.y);
            mesh.maxZ = max(mesh.maxZ, vertex.z);
        }
        else if (prefix == "f")
        {
            Face face;

            iss >> face.v1 >> face.v2 >> face.v3;

            // Obj indexing mulai dari 1, jadi 0 indexing
            face.v1--;
            face.v2--;
            face.v3--;

            mesh.faces.push_back(face);
        }
    }

    if (mesh.vertices.empty())
    {
        cerr << "OBJ file contains no vertices.\n";
        return false;
    }

    return true;
}

CubeInfo computeCube(const Mesh &mesh, float padding) // Sedikit padding supaya nanti saat cek apakah suatu cube overlap pasti kena meshnya
{
    CubeInfo cube;

    cube.cx = (mesh.minX + mesh.maxX) * 0.5f;
    cube.cy = (mesh.minY + mesh.maxY) * 0.5f;
    cube.cz = (mesh.minZ + mesh.maxZ) * 0.5f;

    float dx = mesh.maxX - mesh.minX;
    float dy = mesh.maxY - mesh.minY;
    float dz = mesh.maxZ - mesh.minZ;

    float maxDiff = max(dx, max(dy, dz));

    cube.halfSize = (maxDiff * 0.5f) + padding;

    return cube;
}