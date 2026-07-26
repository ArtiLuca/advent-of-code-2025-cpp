# Advent of Code 2025 — Day 9: Movie Theater

## Part 1

In Part 1, we are given a list of red tiles in a two-dimensional grid.

Each red tile is represented by its coordinates:

```text
x,y
```

Any pair of red tiles can be used as opposite corners of an axis-aligned
rectangle.

The goal is to find the pair of red tiles that forms the rectangle with the
largest area.

Sorting the points does not immediately solve the problem, because the best
rectangle depends on both the horizontal distance and the vertical distance
between two tiles.

For this reason, I used a direct brute-force approach: check every possible pair
of red tiles and keep the maximum area found.

## Idea

Given two red tiles:

```text
(x1, y1)
```

and:

```text
(x2, y2)
```

the rectangle they form has width:

```text
|x2 - x1| + 1
```

and height:

```text
|y2 - y1| + 1
```

The `+1` is needed because the coordinates represent tiles, not just geometric
points. If two red tiles are on coordinates `x = 3` and `x = 5`, the rectangle
includes columns `3`, `4`, and `5`.

Therefore, the area is:

```text
(|x2 - x1| + 1) * (|y2 - y1| + 1)
```

The algorithm simply maximizes this value over all pairs of red tiles.

A possible optimization would be to sort the points and use pruning based on the
largest possible remaining width or height. However, the brute-force approach is
simple and works well for the input size.

## Pseudocode

We model each red tile as a `Point` object with two fields: `x` and `y`.

```text
findMaxArea(T)
    maxArea = 0
    N = T.length

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

The algorithm checks every pair of red tiles, so the number of pairs is:

```text
N(N - 1) / 2
```

Therefore, the time complexity is:

```text
Θ(N^2)
```

The program stores the input points, so the space complexity is:

```text
Θ(N)
```

The solver itself only uses a constant number of extra variables.

---

## Part 2

In Part 2, the list of red tiles has more structure.

Each red tile is connected to the previous and next red tile in the list by a
straight line of green tiles. The list also wraps around, so the first red tile
is connected to the last one.

Adjacent red tiles in the list are always on the same row or the same column.

In other words, the red tiles describe the ordered vertices of a closed
rectilinear polygon: a polygon whose edges are only horizontal or vertical.

The boundary of this polygon is made of red and green tiles, and the inside is
also filled with green tiles.

We still want to find the largest rectangle formed by two red tiles as opposite
corners, but now the rectangle must be entirely made of red and green tiles.

Geometrically, this means that the candidate rectangle must be fully contained
inside the polygon.

## Idea

As in Part 1, we still examine every pair of red tiles.

The difference is that, before using a pair as a candidate answer, we must check
whether the rectangle formed by those two red tiles is valid.

To validate a rectangle, I use two checks.

### 1. Boundary intersection check

If an edge of the polygon crosses through the interior of the candidate
rectangle, then the rectangle cannot be fully inside the polygon.

So, for every polygon edge, I check whether it crosses the strict interior of
the rectangle.

Edges that lie on the boundary of the rectangle are allowed.

### 2. Point-in-polygon check

Even if no polygon edge crosses the rectangle, the rectangle could still be
outside the polygon. For example, it could lie inside the empty gap of a
U-shaped polygon.

To handle this, I test whether the center of the rectangle is inside the
polygon.

I use the standard ray casting idea:

- cast a horizontal ray from the rectangle center to the right;
- count how many polygon edges it crosses;
- if the number of crossings is odd, the point is inside;
- if the number of crossings is even, the point is outside.

If the rectangle passes both checks, it is considered valid.

## Pseudocode

We continue to model each red tile as a `Point`.

We also introduce a `LineSegment` structure to represent each polygon boundary
edge.

Each `LineSegment` stores:

```text
x1, y1
x2, y2
```

## Extracting polygon edges

```text
extractPolygonEdges(T)
    edges = empty list
    N = T.length

    for i = 0 to N - 1
        currentPoint = T[i]
        nextPoint = T[(i + 1) mod N]

        edge = LineSegment(currentPoint, nextPoint)
        append edge to edges

    return edges
```

## Rectangle validation

```text
isValidRectangle(rect, edges)
    for each edge in edges
        if edge crosses the strict interior of rect
            return false

    center = center point of rect
    intersections = 0

    for each edge in edges
        if horizontal ray from center to the right intersects edge
            intersections = intersections + 1

    if intersections is even
        return false

    return true
```

## Main algorithm

```text
findMaxAreaPart2(T)
    maxArea = 0
    edges = extractPolygonEdges(T)
    N = T.length

    for i = 0 to N - 2
        for j = i + 1 to N - 1
            candidateRect = rectangle formed by T[i] and T[j]

            if isValidRectangle(candidateRect, edges)
                width = |T[i].x - T[j].x| + 1
                height = |T[i].y - T[j].y| + 1

                currentArea = width * height

                if currentArea > maxArea
                    maxArea = currentArea

    return maxArea
```

## Complexity

Let `N` be the number of red tiles.

There are:

```text
Θ(N^2)
```

pairs of red tiles.

For each pair, the algorithm validates the candidate rectangle by checking all
polygon edges. Since the polygon has `N` edges, each validation costs:

```text
Θ(N)
```

Therefore, the total time complexity is:

```text
Θ(N^3)
```

The space complexity is:

```text
Θ(N)
```

because the program stores the input points and the polygon edges.

## Files

- `main.cpp` — reads the input and prints both answers
- `day09.h` — declares `Point`, `LineSegment`, and `MovieTheater`
- `day09.cpp` — parses the input and solves both parts

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day09.cpp -o day09
```

## Run

```bash
./day09
```

The program expects the puzzle input in a file named `input.txt`.
