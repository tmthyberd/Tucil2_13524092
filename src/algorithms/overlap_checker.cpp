#include "overlap_checker.hpp"

float min3(float a, float b, float c)
{
    return min(a, min(b, c));
}

float max3(float a, float b, float c)
{
    return max(a, max(b, c));
}

Vec3 subtract(const Vec3 &a, const Vec3 &b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

float dotProduct(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 crossProduct(const Vec3 &a, const Vec3 &b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

float fAbs(float x)
{
    return (x < 0.0f) ? -x : x;
}

NodeState overlapChecker(const Mesh &mesh, const OctreeNode &node)
{
    for (size_t i = 0; i < mesh.faces.size(); i++)
    {
        Vec3 a = mesh.vertices[mesh.faces[i].v1];
        Vec3 b = mesh.vertices[mesh.faces[i].v2];
        Vec3 c = mesh.vertices[mesh.faces[i].v3];

        if (aabbCheck(a, b, c, node))
        {
            if (triangleBoxIntersectionCheck(a, b, c, node))
            {
                return BOUNDARY;
            }
        }
    }

    return EMPTY;
}

bool aabbCheck(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, const OctreeNode &node)
{
    float triMinX = min(v1.x, min(v2.x, v3.x));
    float triMaxX = max(v1.x, max(v2.x, v3.x));

    float triMinY = min(v1.y, min(v2.y, v3.y));
    float triMaxY = max(v1.y, max(v2.y, v3.y));

    float triMinZ = min(v1.z, min(v2.z, v3.z));
    float triMaxZ = max(v1.z, max(v2.z, v3.z));

    float nodeMinX = node.cx - node.halfSize;
    float nodeMaxX = node.cx + node.halfSize;

    float nodeMinY = node.cy - node.halfSize;
    float nodeMaxY = node.cy + node.halfSize;

    float nodeMinZ = node.cz - node.halfSize;
    float nodeMaxZ = node.cz + node.halfSize;

    bool overlapX = (triMaxX >= nodeMinX) && (triMinX <= nodeMaxX);
    bool overlapY = (triMaxY >= nodeMinY) && (triMinY <= nodeMaxY);
    bool overlapZ = (triMaxZ >= nodeMinZ) && (triMinZ <= nodeMaxZ);

    return overlapX && overlapY && overlapZ;
}

bool triangleBoxIntersectionCheck(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, const OctreeNode &node)
{
    float h = node.halfSize;

    Vec3 boxCenter = {node.cx, node.cy, node.cz};

    // Anggap center dari cube adalah origin, segitiga disesuaikan
    Vec3 tv0 = subtract(v1, boxCenter);
    Vec3 tv1 = subtract(v2, boxCenter);
    Vec3 tv2 = subtract(v3, boxCenter);

    // Triangle edges
    Vec3 e0 = subtract(tv1, tv0);
    Vec3 e1 = subtract(tv2, tv1);
    Vec3 e2 = subtract(tv0, tv2);

    // Helper lambda untuk separation axis theorem test
    // TODO: Replace this to not use lambda helper
    auto axisSeparationTest = [&](const Vec3 &axis) -> bool
    {
        // Degenerate axis: skip
        float axisLenSq = dotProduct(axis, axis);
        if (axisLenSq < 1e-8f)
        {
            return false;
        }

        float p0 = dotProduct(tv0, axis);
        float p1 = dotProduct(tv1, axis);
        float p2 = dotProduct(tv2, axis);

        float triMin = min3(p0, p1, p2);
        float triMax = max3(p0, p1, p2);

        float r = h * (fAbs(axis.x) + fAbs(axis.y) + fAbs(axis.z));

        if (triMax < -r || triMin > r)
        {
            return true; // separated
        }

        return false; // overlap on this axis
    };

    // Test 9 axes, triangle edge cross cube normal axes

    // Axis normal ketiga box voxel adalah axis x, y, z
    Vec3 xAxis = {1.0f, 0.0f, 0.0f};
    Vec3 yAxis = {0.0f, 1.0f, 0.0f};
    Vec3 zAxis = {0.0f, 0.0f, 1.0f};

    Vec3 testAxes[9] = {
        crossProduct(e0, xAxis),
        crossProduct(e0, yAxis),
        crossProduct(e0, zAxis),

        crossProduct(e1, xAxis),
        crossProduct(e1, yAxis),
        crossProduct(e1, zAxis),

        crossProduct(e2, xAxis),
        crossProduct(e2, yAxis),
        crossProduct(e2, zAxis)};

    for (int i = 0; i < 9; i++)
    {
        if (axisSeparationTest(testAxes[i]))
        {
            return false;
        }
    }

    // Test 3 distinct cube faces normal axis

    float minX = min3(tv0.x, tv1.x, tv2.x);
    float maxX = max3(tv0.x, tv1.x, tv2.x);
    if (maxX < -h || minX > h)
    {
        return false;
    }

    float minY = min3(tv0.y, tv1.y, tv2.y);
    float maxY = max3(tv0.y, tv1.y, tv2.y);
    if (maxY < -h || minY > h)
    {
        return false;
    }

    float minZ = min3(tv0.z, tv1.z, tv2.z);
    float maxZ = max3(tv0.z, tv1.z, tv2.z);
    if (maxZ < -h || minZ > h)
    {
        return false;
    }

    // Test axis normal triangle

    Vec3 triNormal = crossProduct(e0, e1);

    if (axisSeparationTest(triNormal))
    {
        return false;
    }

    return true;
}
