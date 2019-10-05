// 3D point space: cubic with this side length (in meters)
#define SPACEEDGE 10

#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100
// Assume all voxels are cubic of this side length (in meters):
#define VOXELSIZE 0.1

// Length on the ray to be looked at on either side of the point
// Arbitrarily choose 2 voxels
#define DELTA 0.2

/* Voxel grid geometry
In front of the camera: x, extends all 100 boxes from origin in +x direction
Left/right of the camera: y, extends 50 boxes in +y (right) and 50 in -y
Up/down from the camera: z, extends 50 boxes in +z (up) and 50 in -z
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.c"
#include "matrix.c"

void initializeVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], const int width, const int height, const int depth) {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            for(int k = 0; k < width; k++) {
                grid[i][j][k] = 0;
            }
        }
    }
}

void castRay(double ray[3], const double point[3], const double cam[3]) {
    double unscaledRay[3];
    double scaledRay[3];
    vecSubtract(3, point, cam, unscaledRay);
    vecUnit(3, unscaledRay, scaledRay);
}

void incrementVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], double location[3]) {
    // Map location to voxel grid coordinates
    int boxX = floor(location[0] / VOXELSIZE);
    int boxY = floor(location[1] / VOXELSIZE);
    int boxZ = floor(location[2] / VOXELSIZE);
    int voxelCoordinates[3];
    vec3set(boxX, boxY, boxZ, voxelCoordinates);

    // Since the camera is at voxel locations x=0, y=max/2, z=max/2, we need to
    // offset our coordinates to match.
    int camCoordinateOffset[3];
    vec3set(0, Y_BOXES/2, Z_BOXES/2, &camCoordinateOffset);
    int gridCoordinates[3];
    vecAdd(3, camCoordinateOffset, voxelCoordinates, gridCoordinates);

    // Ensure that the coordinate is inside the voxel grid
    if (gridCoordinates[0] >= X_BOXES) {
        exit(1);
    }
    if (gridCoordinates[1] >= Y_BOXES) {
        exit(1);
    }
    if (gridCoordinates[2] >= Z_BOXES) {
        exit(1);
    }
}

void main() {
    int voxelGrid[X_BOXES][Y_BOXES][Z_BOXES];

    // Camera defined to be at origin, pointed directly in +x direction
    double cam[3];
    vec3set(0, 0, 0, &cam);

    // The position of an arbitrary point in camera space, coordinates in meters
    double point[3];
    vec3Set(1.3, 4.2, 7.3, &point);


}