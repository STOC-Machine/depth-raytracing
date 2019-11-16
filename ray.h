// This many voxels per dimension
#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100


void getVoxelCoordinates(const double location[3], int voxelCoordinates[3]);

void initializeVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], const int width, const int height, const int depth);

int findLimitingCoordinate(const double delta[3], const double pointUnit[3]);

void incrementVoxelGrid(int grid[X_BOXES][Y_BOXES][Z_BOXES], int coordinates[3],
        int increment);

void castPointRay(double ray[3], const double point[3], const double cam[3]);

/* Finds the new distances to the next voxel boundaries, after a cast limited by one coordinate.

Returns the total distance travelled by the ray. This can be used for checking whether the ray has travelled far enough.
*/
double castToNextVoxel(double delta[3], const double pointUnit[3],
        int voxelCoordinates[3], const int limitingCoord);

void castPositiveDelta(const double point[3], int grid[X_BOXES][Y_BOXES][Z_BOXES]);

void castNegativeDelta(const double point[3], int grid[X_BOXES][Y_BOXES][Z_BOXES]);
