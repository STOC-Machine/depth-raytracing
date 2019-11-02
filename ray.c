// 3D point space: cubic with this side length (in meters)
#define SPACEEDGE 10

// This many voxels per dimension
#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100
// Assume all voxels are cubic of this side length (in meters):
// dividing SPACEEDGE / BOXES
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
#include <assert.h>
#include "vector.c"
#include "matrix.c"

void getVoxelCoordinates(const double location[3], int voxelCoordinates[3]) {
    // Map location to voxel grid coordinates
    voxelCoordinates[0] = floor(location[0] / VOXELSIZE) + (X_BOXES / 2);
    voxelCoordinates[1] = floor(location[1] / VOXELSIZE) + (Y_BOXES / 2);
    voxelCoordinates[2] = floor(location[2] / VOXELSIZE) + (Z_BOXES / 2);
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

int findLimitingCoordinate(const double delta[3], const double pointUnit[3]) {
    double nextBoundaryLength[3];
    int limitingCoordinate = 0;
    for (int i = 0; i < 3; i++) {
        if (fabs(delta[i]) >= fabs(pointUnit[i] * DELTA)) {
            nextBoundaryLength[i] = DELTA + 1;
        } else {
            nextBoundaryLength[i] = delta[i] / pointUnit[i];
            // printf("I'm doing division!!!!!!!!!!!!!!!!!!\n");
        }
        assert(nextBoundaryLength >= 0 && "nextBoundaryLength negative?");
        // Check for overflows and divide by 0
        // printf("delta, q=%i: %f\n", i, delta[i]);
        // printf("pointUnit, q=%i: %f\n", i, pointUnit[i]);
        // printf("nextBoundaryLength, q=%i: %f\n", i, nextBoundaryLength[i]);
        // printf("delta[i] / pointUnit[i]:      %f\n", delta[i] / pointUnit[i]);
        // printf("fabs(delta[i] / pointUnit[i]): %f\n", fabs(delta[i] / pointUnit[i]));

        // Select smallest coordinate: if this is smaller than the previous
        // smallest length, use it. This behavior is ill-defined for the 0th
        // iteration, but it doesn't matter because limitingCoordinate will be 0
        // at the end no matter what
        if (nextBoundaryLength[i] < nextBoundaryLength[limitingCoordinate]) {
            limitingCoordinate = i;
        }
    }
    printf("Limiting coordinate: %i\n", limitingCoordinate);
    return limitingCoordinate;
}

void incrementVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], int coordinates[3],
        int increment) {
    // Ensure that the coordinate is inside the voxel grid
    if (coordinates[0] >= X_BOXES || coordinates[0] < 0) {
        printf("Invalid voxel coordinate: (%i, %i, %i)\n",
            coordinates[0], coordinates[1], coordinates[2]);
        exit(1);
    }
    if (coordinates[1] >= Y_BOXES || coordinates[1] < 0) {
        printf("Invalid voxel coordinate: (%i, %i, %i)\n",
            coordinates[0], coordinates[1], coordinates[2]);
        exit(1);
    }
    if (coordinates[2] >= Z_BOXES || coordinates[2] < 0) {
        printf("Invalid voxel coordinate: (%i, %i, %i)\n",
            coordinates[0], coordinates[1], coordinates[2]);
        exit(1);
    }
    grid[coordinates[0]][coordinates[1]][coordinates[2]] += increment;

    printf("Incrementing voxel (%i, %i, %i) by %i.\n",
        coordinates[0], coordinates[1], coordinates[2], increment);
}

void castPointRay(double ray[3], const double point[3], const double cam[3]) {
    vecSubtract(3, point, cam, ray);
}

/* Finds the new distances to the next voxel boundaries, after a cast limited by one coordinate.

Returns the total distance travelled by the ray. This can be used for checking whether the ray has travelled far enough.
*/
double castToNextVoxel(double delta[3], const double pointUnit[3],
        int voxelCoordinates[3], const int limitingCoord) {

    // Update delta (distances to next voxels in each dimension)
    double vectorTravelled[3];
    double distanceTravelled = delta[limitingCoord] / pointUnit[limitingCoord];
    assert(distanceTravelled >= 0 && "Negative distance travelled?");
    vecScale(3, distanceTravelled, pointUnit, vectorTravelled);
    vecSubtract(3, delta, vectorTravelled, delta);
    if (pointUnit[limitingCoord] > 0) {
        delta[limitingCoord] = VOXELSIZE;
    } else {
        delta[limitingCoord] = -VOXELSIZE;
    }

    // Increment voxel coordinates by crossing the boundary
    if (pointUnit[limitingCoord] < 0) {
        voxelCoordinates[limitingCoord] -= 1;

    } else {
        voxelCoordinates[limitingCoord] += 1;
    }

    // Return distance travelled in this iteration
    return distanceTravelled;
}

void castPositiveDelta(const double point[3], int grid[X_BOXES][Y_BOXES][Z_BOXES]) {
    double pointUnit[3];
    vecUnit(3, point, pointUnit);
    // First iteration: special case, since it's at the point p instead of
    // a voxel grid boundary
    // Set delta, the vector cast from the current point to the next voxel
    // boundary, from the initial point. Note, this works with negative
    // locations. If a coordinate is negative ("below" the camera, assumed to be
    // at the origin for now), its delta will be negative coming from fmod. This
    // is what we want, since the distance to be travelled is in the negative
    // direction.
    double delta[3];
    double deltaX = fmod(point[0], VOXELSIZE);
    double deltaY = fmod(point[1], VOXELSIZE);
    double deltaZ = fmod(point[2], VOXELSIZE);
    vec3Set(deltaX, deltaY, deltaZ, delta);
    // Set voxel coordinates at intitial point
    int voxelCoordinates[3];
    getVoxelCoordinates(point, voxelCoordinates);
    incrementVoxelGrid(grid, voxelCoordinates, 1);
    int limitingCoordinate = findLimitingCoordinate(delta, pointUnit);
    double distanceTravelled = castToNextVoxel(delta, pointUnit, voxelCoordinates, limitingCoordinate);
    printf("Total distance travelled: %f\n\n", distanceTravelled);

    // Remaining iterations
    assert(distanceTravelled <= sqrt(2)*DELTA && "Delta ray cast does not leave a voxel?");
    while (distanceTravelled < DELTA) {
        // Increment the voxel that has just been entered
        incrementVoxelGrid(grid, voxelCoordinates, 1);

        // Cast until the next voxel
        limitingCoordinate = findLimitingCoordinate(delta, pointUnit);
        distanceTravelled += castToNextVoxel(delta, pointUnit, voxelCoordinates, limitingCoordinate);
        printf("Total distance travelled: %f\n", distanceTravelled);
        printf("\n");
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

    printf("floor(-2.3) = %f\n", floor(-2.3));

    // Camera defined to be at origin, pointed directly in +x direction
    double cam[3];
    vec3Set(0.0, 0.0, 0.0, cam);
    // The position of an arbitrary point in camera space, coordinates in meters
    double point[3];
    vec3Set(-1.3, -4.2, -2.3, point);
    printf("Testing. With point:  (%f, %f, %f),\n", point[0], point[1], point[2]);
    printf("              camera: (%f, %f, %f).\n", cam[0], cam[1], cam[2]);
    int pointVoxelTest[3];
    getVoxelCoordinates(point, pointVoxelTest);
    printf("Point is at voxel (%i, %i, %i).\n\n",
        pointVoxelTest[0], pointVoxelTest[1], pointVoxelTest[2]);
    castPositiveDelta(point, voxelGrid);
    //TODO test 0 component

    vec3Set(-1.3, 0, 0, point);
    printf("Testing. With point:  (%f, %f, %f),\n", point[0], point[1], point[2]);
    printf("              camera: (%f, %f, %f).\n", cam[0], cam[1], cam[2]);
    pointVoxelTest[3];
    getVoxelCoordinates(point, pointVoxelTest);
    printf("Point is at voxel (%i, %i, %i).\n\n",
        pointVoxelTest[0], pointVoxelTest[1], pointVoxelTest[2]);
    castPositiveDelta(point, voxelGrid);
    //TODO test 0 component
}
