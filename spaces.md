# Design of Spaces and Transformations

## List of Spaces

There are essentially 3 spaces we need to worry about:
- A "canonical space" that can be used by the algorithm
    - Call this `S`?
- Voxel space, which maps all of canonical space from the origin to arbitrary numbers of voxels
    - Call this `V`?
- The space coming out of the RealSense. We don't know what to do with this, but we currently assume that we will map it to canonical space. In current implementations, we assume that it is the same as canonical space, although this assumption almost certainly doesn't hold.

## Mapping S -> V: Canonical to Voxel Spaces

This follows essentially the same process we did in computer graphics with orthogonal transformations. (But that's even more general, which may be a good thing.) In that, we took a viewing volume prism, mapped it to "normalized coordinates" (which run from `(-1,-1,-1)` to `(1,1,1)`). Then we mapped that to "screen coordinates", which run from `(0,0,0)` to an arbitrary `(x,y,z)`. Essentially, this is what we want to do to map from the arbitrary size volume of canonical space into voxel space, which runs from the origin to an arbitrary number of voxels.

According to Josh, the projection transformation (from camera to normalized coordinates) is 
```
{
 {2/(r - l), 0,         0,          (-r - l)/(r - l)},
 {0,         2/(t - b), 0,          (-t - b)/(t - b)},
 {0,         0,         -2/(n - f), (n + f)/(n - f)},
 {0,         0,         0,          1}
}
```
in a homogeneous transformation. (that's in Mathematica matrix format!)

The viewport transformation (from normalized to screen coordinates) is 
```
{
 {x/2, 0, 0, x/2},
 {0, y/2, 0, y/2},
 {0, 0, 1/2, 1/2},
 {0, 0, 0, 1}
}
```
This applies for when the z boundary is set at z=1. Since we want an arbitrary value, it follows that the same pattern gives us
```
{
 {x/2, 0, 0, x/2},
 {0, y/2, 0, y/2},
 {0, 0, z/2, z/2},
 {0, 0, 0, 1}
}
```

Multiplying the two transformations (projection, then viewport) will give us the overall transformation we want. This is 
```
x/(-l+r)    0           0       (l x)/(l-r)
0           y/(-b+t)    0       (b y)/(b-t)
0           0           z/(f-n) -((n z)/(f-n))
0           0           0       1
```

In order to implement this transformation, all we need is the corners of space and the number of voxels. Then, after the final transformation is applied, we will cast down each coordinate (in the non-homogeneous matrix, for speed) to obtain integer indices we can use in the C array.
