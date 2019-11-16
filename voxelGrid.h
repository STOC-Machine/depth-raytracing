#ifndef _VOXELGRID
#define _VOXELGRID

#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100

typedef struct voxelGrid voxelGrid;
struct voxelGrid {
    //TODO remove hard-coded grid size
    int voxels[X_BOXES][Y_BOXES][Z_BOXES];

    double coordinateTransformIsometry[4][4];
    double invCoordinateTransformIsometry[4][4];
};

void gridInitialize(voxelGrid *grid,
                    double nearX, double nearY, double nearZ,
                    double farX, double farY, double farZ);

#endif
