#include "voxel_subdivider.hpp"

void voxelSubdivider(OctreeNode &root, int maxDepth, const Mesh &mesh)
{
    // Syarat2 rekursif berhenti
    if (root.depth >= maxDepth)
        return;
    if (root.state == EMPTY)
        return;

    const float childHalfSize = root.halfSize * 0.5f;
    int childIndex = 0;

    for (int sx = -1; sx <= 1; sx += 2)
    {
        for (int sy = -1; sy <= 1; sy += 2)
        {
            for (int sz = -1; sz <= 1; sz += 2)
            {
                root.children[childIndex] = new OctreeNode(
                    root.cx + childHalfSize * sx,
                    root.cy + childHalfSize * sy,
                    root.cz + childHalfSize * sz,
                    childHalfSize,
                    root.depth + 1);

                root.children[childIndex]->state =
                    overlapChecker(mesh, *root.children[childIndex]);

                voxelSubdivider(*root.children[childIndex], maxDepth, mesh); // Rekursif sampai akhir

                childIndex++;
            }
        }
    }
}