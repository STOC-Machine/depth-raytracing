#include "voxelGrid.h"

/* Initializes a voxel grid with all data entries as 0.
 *
 * Parameters:
 * voxelGrid *grid: the grid to be initialized
 */
void gridInitializeData(voxelGrid *grid) {
    // Initialize entire grid to 0
    for(int x = 0; x < X_BOXES; x++) {
        for(int y = 0; y < Y_BOXES; y++) {
            for(int z = 0; z < Z_BOXES; z++) {
                (grid->voxels)[x][y][z] = 0;
            }
        }
    }
}

/* Initializes the transformation from space to voxel coordinates.
 *
 * This transformation can be multiplied by a coordinate vector to get its location in voxel space. The resulting coordinates should then be cast to integers using floor, so they can be used to index the C array.
 */
void gridInitializeTransformation(voxelGrid *grid,
                                  double nearX, double nearY, double nearZ,
                                  double farX, double farY, double farZ) {
    double isometry[4][4] = grid->coordinateTransformIsometry;

    // Initialize matrix to all zeros, which will then be added to
    mat44Zero(isometry);
    // Initialize rotation/scaling components
    isometry[0][0] = X_BOXES / (-nearX + farX);
    isometry[1][1] = Y_BOXES / (-nearY + farY);
    isometry[2][2] = Z_BOXES / (farZ - nearZ);
    // Initialize translation components
    isometry[0][3] = nearX * X_BOXES / (nearX - farX);
    isometry[1][3] = nearY * Y_BOXES / (nearY - farY);
    isometry[2][3] = -nearZ * Z_BOXES / (nearZ - farZ);
    // Initialize isometry components
    isometry[3][3] = 1;
}

void gridInitialize(voxelGrid *grid,
                    double nearX, double nearY, double nearZ,
                    double farX, double farY, double farZ) {
    gridInitializeData(grid);
    gridInitializeTransformation(grid, nearX, nearY, nearZ, farX, farY, farZ);
}
