# Original Notes — Day 9: Movie Theater

> These are my original notes for this day, kept to show my first reasoning before cleaning the explanation into the final `README.md`.

## Part 1

In Part 1, we are given a list containing the coordinates `(x, y)` of red tiles in a 2D grid.

These tiles can be used in pairs as the opposite corners of an axis-aligned rectangle.

We want to find the two tiles that form the rectangle with the largest area.

Sorting does not immediately solve the problem, and neither does an approach based only on finding the tiles closest to the smallest and largest coordinates of the 2D grid. The area depends on both the horizontal and vertical distance between the two chosen tiles.

Therefore, for Part 1, I decided to use a brute-force approach with nested loops to find the maximum area.

## Idea

Given a pair of red tiles $(x_1,y_1)$ and $(x_2,y_2)$, the area of the rectangle they form is:

```math
\left(|x_2-x_1|+1\right)
\left(|y_2-y_1|+1\right).
```

The `+1` is necessary because the coordinates represent tiles rather than geometric lengths. For example, the coordinates `x = 3` and `x = 5` include the three tile columns `3`, `4`, and `5`.

Therefore, the largest area formed by any two red tiles can be calculated by maximizing this formula across all pairs.

A possible optimization would be to sort the points and use bounding-box pruning. To prune safely, the pairs would need to be processed in an order where the maximum possible area of the remaining candidates can only decrease. If that upper bound could not exceed the current maximum, the remaining candidates could be skipped.

However, I did not implement this optimization, as the brute-force approach of testing every possible pair works well for the input size.

## Pseudocode

We model each red tile as a `Point` object containing `x` and `y` fields to represent its 2D coordinates.

```text
// T[0..N-1] contains the Point objects
// exactly as read from the input
findMaxArea(T)
    N = T.length
    maxArea = 0

    // Check every possible unordered pair
    for i = 0 to N - 2
        for j = i + 1 to N - 1
            width = |T[i].x - T[j].x| + 1
            height = |T[i].y - T[j].y| + 1
            currentArea = width * height

            if currentArea > maxArea
                maxArea = currentArea

    return maxArea
```

## Complexity

Let `N` be the number of red tiles.

The algorithm checks every possible unordered pair. The exact number of pairs is:

```math
\frac{N(N-1)}{2}.
```

Therefore, the time complexity is:

```math
\Theta(N^2).
```

The program stores the `N` input points, requiring:

```math
\Theta(N)
```

space.

The Part 1 solver itself uses only a constant number of additional variables, so its auxiliary space complexity beyond the stored input is:

```math
\Theta(1).
```

---

## Part 2

In Part 2, we are told that every red tile in the list is connected to the tile before it and the tile after it by a straight line of green tiles.

The list wraps around, so the first red tile is also connected to the last red tile.

We are also told that tiles that are adjacent in the list are always on either the same row or the same column.

In other words, the list of red tiles represents the ordered vertices of a closed rectilinear polygon: a polygon whose edges are strictly horizontal or vertical.

The tiles connecting these vertices are green, and the entire interior of the polygon is also filled with green tiles.

We still need to find the largest rectangle formed by two red tiles as its opposite corners, but with the additional constraint that the chosen rectangle must be entirely composed of red and green tiles.

In geometric terms, this means that the candidate rectangle must be fully contained within the polygon, although its boundary may lie on the boundary of the polygon.

## Idea

We can still use the brute-force approach to evaluate every pair of red tiles, given the manageable size of the input.

However, before considering the area of a rectangle formed by two tiles, we need to validate that the rectangle is completely contained within the rectilinear polygon.

For this approach, we check two conditions.

### 1. Boundary Intersection

No edge of the polygon may cross through the strict interior of the candidate rectangle.

An edge that lies on the boundary of the rectangle is allowed, but an edge that cuts through its interior means that part of the rectangle lies outside the polygon.

Because the polygon is rectilinear, each edge is either horizontal or vertical:

* for a horizontal edge, we check whether its `y` coordinate lies strictly between the bottom and top of the rectangle and whether its horizontal interval overlaps the rectangle's interior;
* for a vertical edge, we perform the corresponding check using its `x` coordinate and vertical interval.

### 2. Point-in-Polygon with Ray Casting

Even if no polygon edge crosses the rectangle, the rectangle could still lie entirely in empty space outside the polygon.

For example, it could bridge the empty gap of a U-shaped polygon.

Therefore, we test whether the centre of the candidate rectangle lies inside the polygon.

We cast a horizontal ray from the centre towards the right and count how many vertical polygon edges it crosses:

* if the number of intersections is odd, the point is inside the polygon;
* if the number of intersections is even, the point is outside the polygon.

The implementation slightly shifts the vertical position of the ray to avoid passing exactly through a polygon vertex.

If a candidate rectangle passes both checks, we consider it valid and calculate its area.

## Pseudocode

We continue to model each red tile as a `Point`.

We also introduce a `LineSegment` structure to represent the green boundary edges connecting consecutive points.

Each `LineSegment` stores:

```text
x1, y1
x2, y2
```

### Extracting the Polygon Edges

```text
// T[0..N-1] contains the ordered polygon vertices
extractPolygonEdges(T)
    edges = empty list
    N = T.length

    for i = 0 to N - 1
        currentPoint = T[i]

        // Wrap the last point back to the first
        nextPoint = T[(i + 1) mod N]

        newEdge = LineSegment(
            currentPoint.x,
            currentPoint.y,
            nextPoint.x,
            nextPoint.y
        )

        append newEdge to edges

    return edges
```

### Validating a Rectangle

Assume that `p1` and `p2` are the two opposite corners of the candidate rectangle.

```text
isValidRectangle(p1, p2, edges)
    minX = min(p1.x, p2.x)
    maxX = max(p1.x, p2.x)
    minY = min(p1.y, p2.y)
    maxY = max(p1.y, p2.y)

    // 1. Check whether any polygon edge crosses
    // the strict interior of the rectangle
    for each edge in edges

        // Horizontal polygon edge
        if edge.y1 == edge.y2
            edgeY = edge.y1
            edgeMinX = min(edge.x1, edge.x2)
            edgeMaxX = max(edge.x1, edge.x2)

            if minY < edgeY and edgeY < maxY
                if edgeMinX < maxX and edgeMaxX > minX
                    return false

        // Vertical polygon edge
        else if edge.x1 == edge.x2
            edgeX = edge.x1
            edgeMinY = min(edge.y1, edge.y2)
            edgeMaxY = max(edge.y1, edge.y2)

            if minX < edgeX and edgeX < maxX
                if edgeMinY < maxY and edgeMaxY > minY
                    return false

    // 2. Point-in-polygon check using ray casting
    midX = (p1.x + p2.x) / 2.0
    midY = (p1.y + p2.y) / 2.0

    // Small offset to avoid passing exactly through a vertex
    rayY = midY + 0.1
    intersections = 0

    for each edge in edges

        // A horizontal ray only crosses vertical edges
        if edge.x1 == edge.x2
            edgeX = edge.x1
            edgeMinY = min(edge.y1, edge.y2)
            edgeMaxY = max(edge.y1, edge.y2)

            if edgeX > midX
               and edgeMinY < rayY
               and rayY < edgeMaxY

                intersections = intersections + 1

    // Odd means inside; even means outside
    return intersections mod 2 != 0
```

### Part 2 Solution

```text
findMaxAreaPart2(T)
    N = T.length

    if N < 4
        return 0

    maxArea = 0
    edges = extractPolygonEdges(T)

    // Check every possible pair
    for i = 0 to N - 2
        for j = i + 1 to N - 1

            if isValidRectangle(T[i], T[j], edges)
                width = |T[i].x - T[j].x| + 1
                height = |T[i].y - T[j].y| + 1
                currentArea = width * height

                if currentArea > maxArea
                    maxArea = currentArea

    return maxArea
```

## Complexity

Let `N` be the number of red tiles and therefore also the number of polygon edges.

Extracting the polygon edges takes:

```math
\Theta(N)
```

time and space.

There are:

```math
\frac{N(N-1)}{2}=\Theta(N^2)
```

pairs of red tiles.

For every pair, the rectangle-validation procedure may scan all `N` polygon edges during the boundary-intersection check and then scan them again during ray casting.

The two scans still require:

```math
\Theta(N)
```

time per candidate rectangle in the worst case.

Therefore, the worst-case time complexity is:

```math
\Theta(N^3).
```

The program stores the `N` input points and the `N` polygon edges.

Therefore, the total space complexity is:

```math
\Theta(N).
```

The edge list is the main additional structure created specifically for Part 2, so the auxiliary space beyond the stored input points is also:

```math
\Theta(N).
```
