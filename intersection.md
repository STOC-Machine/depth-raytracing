# The Intersection Algorithm

The idea is to extend a vector along the camera->point vector from the point. Then, we increment every voxel that intersects that vector.
Voxels farther from the camera are incremented by -1 (to create a negative number) and closer to the camera are incremented by +1.

Once we have the vector (call it δ): we want to figure out which voxels it intersects.

The basic idea is a "least travel time" test between vertical and horizontal voxel boundaries. If the travel time is shorter to a horizontal boundary, it intersects that first and will go into the vertically next voxel (ie up or down). Similarly, if it's shorter to a vertical boundary, then the vector will proceed to the horizontally next voxel (out or in, left or right, depending on your perspective).

- Start with the point. Find the location of the next intersection.
  - Is it farther to the horizontal or vertical boundary? We can figure this out with a simple modulo with the voxel size, I would imagine.
- If it's at a horizontal boundary: then the vertical distance to the next voxel is D (the edge size). If it's at a vertical boundary, then the horizontal distance to the next voxel is D.
  - We should then calculate the other distance. We know the distance traveled by the ray from the last intersection. If it was a horizontal boundary, then we can find this total distance from the vertical distance. Say we have current travel and last travel buffers.
  - Use this to find the horizontal distance to the next voxel. Since a horizontal boundary means that we're traveling incrementally horizontally (on a line like this: `------`), this is what will be incremented by a different amount each time.
    - If it so happens that we travel a whole D in one direction, then we know exactly what the distance travelled in the other one is ahead of time. But this is an edge case and probably not worth accounting for (since the advantages from doing that would probably be offset by the penalty of checking for it)

Let's use an example.

```
+----------+----------+----------+----------+
|          |          |          |          |
|          |          |          |          |
|          |          |          |          |
|          |          |          |          |
+------------------------X-------+----------+
|          |          |          X          |
|          |          |          |          X <-- Start here
|          |          |          |          |
|          |          |          |          |
+-------------------------------------------+
|          |          |          |          |
|          |          |          |          |
|          |          |          |          |
|          |          |          |          |
+----------+----------+----------+----------+
<--- D ---->

             y
             ^
             |
             |
x <----------+
```

- First, we hit the one on the far right. This is a vertical boundary, so limited by Δx.
  - From this, we find Δy.
  - Then, we calculate the remaining distance to the next voxels.
    - x: this distance is just D
    - y: this distance is the previous remaining distance minus Δy.
- Then, we hit the vertical boundary at the second X.
  - This is a vertical boundary, so limited by Δx.
  - We use `Δx = D`, from the result of the last increment.
  - Then, we find Δy from this.
  - Then, we calculate the remaining distance to the next voxel.
    - x: this distance is just D (again)
    - y: this is the previous remaining distance minux Δy

Eventually, we must be beyond the δ vector. At that point, stop everything. Intersect the current voxel, because we must have passed out of the vector after the last voxel intersection, so it would happen inside this voxel. (**This wouldn't necessarily be true if it ends exactly on a voxel boundary**)

How do I deal with negative increments in either direction? (This must happen, since I cast vectors in opposite directions)

So what goes into the algorithm?

## Algorithm (for positive δ iterations)

- First iteration: check both x and y
  - Modulo coordinates of p by D: get the distance to travel in x and y directions
  - If Δx < Δy:
    - Will hit an X (vertical) boundary
  - Else:
    - Will hit a Y (horizontal) boundary
  - Increment the current voxel
    - Divide each coordinate of p by D and round down
    - So calling the voxel coordinate vector g:
      - `gx = floor(px / D)`
      - `gy = floor(py / D)`
- Further iterations:
  - If x > proj (p + δ, x) or y > proj (p + δ, y):
    - This vector cast has led us outside δ. Ignore the current grid.
  - If Δx < Δy:
    - Will hit an X (vertical) boundary
    - Modify Δx, Δy: update Δ
  - Else:
    - Will hit a Y (horizontal) boundary
    - Modify Δx, Δy: update Δ
  - Increment the next voxel (which is just being entered at the boundary)
     - To get the grid location:
        - I should store this in a vector (say `g`).
        - If I hit a y boundary, then I have entered a new y voxel.
          - Increment g_y by +1
          - Change voxel value at that location
        - If I hit an x boundary, then I have entered a new x voxel.
          - Increment g_x by +1
          - Change voxel value at that location
- To get initial value of g, divide coordinates of p by D and round down

### How to find limiting coordinate

- The "time to travel" in each coordinate is `Δq/uq`. It is the scale factor of `u`, so essentially the length of the vector that will get to the next q intersection.
- So the first intersection is the coordinate where this is smallest.
- If this is greater than `δ` (the distance to be travelled on either side of the point), we can ignore that coordinate permanently. The boundary will never be hit. (So if `Δq/uq > δ`)
- If a coordinate in `u` is 0, then we have a divide by 0 error. We need to check for this manually.
- To account for this, we can implement a check that `Δq >= uq * δ`. This is just rearranging the equation above. It also handles the divide by 0 case in a way that's less likely to overflow.
- So, the algorithm:
  - Calculate the scale values (call it `nextBoundaryLength`?)
  - Run the check. If `Δq >= uq * δ`, then set its `nextBoundaryLength` to a large number (say, `δ + 1`) so it will not be chosen.
    - Do we need to check that this works? Will we be guaranteed that at least one coordinate will be less than δ? *It should*.

### A new way of updating Δ by vectors

- This should be much nicer to work with in 3D, although a little slower than if we optimized perfectly to ignore unnecessary calculations
- Input: the vector Δ0 (the distances to the voxel boundaries before the latest iteration), the vector u (a unit vector in the direction of the ray), the limiting coordinate q
- Like above, the new `Δ = Δ0 - d`, where d is the vector traveled along the path to the voxel boundary
- d is u scaled to match the limiting Δq
  - `d = (Δq0 / uq) u`
- Then, set `Δq = D`, since it now has to travel across an entire voxel in the q direction (otherwise, the value should say 0)

## Testing

Assuming camera starts at (0,0) in voxel and coordinate space

**What happens if Δx = Δy**?
**What happens with pure vertical or horizontal rays?**