// 3D point space: cubic with this side length (in meters)
#define SPACEEDGE 10

#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100
// Assume all voxels are cubic of this side length (in meters):
#define VOXELSIZE 0.1

// Length on the ray to be looked at on either side of the point
// Arbitrarily choose 3 * voxel edge length
#define DELTA 0.3

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

void getVoxelCoordinates(const double location[3], int voxelCoordinates[3]) {
    // Map location to voxel grid coordinates
    voxelCoordinates[0] = floor(location[0] / VOXELSIZE);
    voxelCoordinates[1] = floor(location[1] / VOXELSIZE);
    voxelCoordinates[2] = floor(location[2] / VOXELSIZE);
}

void initializeVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], const int width, const int height, const int depth) {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            for(int k = 0; k < width; k++) {
                grid[i][j][k] = 0;
            }
        }
    }
}

void incrementVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], int coordinates[3],
        int increment) {
    // Ensure that the coordinate is inside the voxel grid
    if (coordinates[0] >= X_BOXES || coordinates[0] < 0) {
        exit(1);
    }
    if (coordinates[1] >= Y_BOXES || coordinates[1] < 0) {
        exit(1);
    }
    if (coordinates[2] >= Z_BOXES || coordinates[2] < 0) {
        exit(1);
    }
    grid[coordinates[0]][coordinates[1]][coordinates[2]] += increment;
}

void castPointRay(double ray[3], const double point[3], const double cam[3]) {
    vecSubtract(3, point, cam, ray);
}


void updateDelta(double delta[3], const double pointUnit[3], const int limitingCoord) {
     double vectorTravelled[3];
     vecScale(3, delta[limitingCoord] / pointUnit[limitingCoord],
        delta, vectorTravelled);
    vecSubtract(3, delta, vectorTravelled, delta);
    delta[limitingCoord] = VOXELSIZE;
}

void castPositiveDelta(const double point[3], int grid[X_BOXES][Y_BOXES][Z_BOXES]) {
    double pointUnit[3];
    vecUnit(3, pointUnit, pointUnit);
    // First iteration: special case, since it's at the point p instead of
    // a voxel grid boundary
    // Set delta, the vector cast from the current point to the next voxel boundary,
    // from the initial point
    double delta[3];
    double deltaX = fmod(point[0], VOXELSIZE);
    double deltaY = fmod(point[1], VOXELSIZE);
    double deltaZ = fmod(point[2], VOXELSIZE);
    vec3Set(deltaX, deltaY, deltaZ, delta);
    // Set voxel coordinates at intitial point
    int voxelCoordinates[3];
    getVoxelCoordinates(point, voxelCoordinates);
    incrementVoxelGrid(grid, voxelCoordinates, 1);

    if (delta[0] < delta[1] && delta[0] < delta[2]) {
        // Hits x (0 index) boundary first
        updateDelta(delta, pointUnit, 0);

    } else if (delta[1] < delta[0] && delta[1] < delta[2]) {
        // Hits y (1 index) boundary first
        updateDelta(delta, pointUnit, 1);
    } else if (delta[2] < delta[0] && delta[2] < delta[1]) {
        // Hits z (2 index) boundary first
        updateDelta(delta, pointUnit, 2);
    }
}

int main() {
    printf("Hello there, I am a raytracer!\n");
    int voxelGrid[X_BOXES][Y_BOXES][Z_BOXES];
    initializeVoxelGrid(voxelGrid, X_BOXES, Y_BOXES, Z_BOXES);


    // To test:
    // set a camera and arbitrary point in space to be intersected with
    // the ray algorithm.
    // Cast a ray and run the intersection code

    // Camera defined to be at origin, pointed directly in +x direction
    double cam[3];
    vec3Set(0.0, 0.0, 0.0, cam);
    // The position of an arbitrary point in camera space, coordinates in meters
    double point[3];
    vec3Set(1.3, 4.2, 7.3, point);
}
