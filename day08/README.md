# Advent of Code 2025 — Day 8: Playground

## Part 1

In Part 1, we are dealing with junction boxes.

The input gives a list of junction boxes, each represented by three-dimensional
coordinates:

```text
x,y,z
```

The goal is to repeatedly consider pairs of junction boxes in order of
increasing distance.

When two junction boxes are connected, they become part of the same circuit. If
one of them already belongs to a circuit, the other one joins that circuit. If
both boxes are already in the same circuit, nothing changes.

After processing the first `1000` closest candidate connections, we must find
the sizes of the three largest circuits and multiply them together.

## Note

At first, I thought about using a priority queue or min-heap to avoid generating
all possible pairs.

However, every possible pair of junction boxes may be relevant, so the program
still needs to compare many pairwise distances. For an input of size `N`, there
are:

```text
N(N - 1) / 2
```

unique pairs.

For `N = 1000`, this is:

```text
1000 × 999 / 2 = 499500
```

pairs, which is large but still manageable.

Because of this, I decided to generate all unique edges, sort them by distance,
and then process them in sorted order.

There are more advanced geometric approaches that could reduce the amount of
work, such as spatial hashing or k-d trees, but for this puzzle I chose the
clearer graph-based solution.

## Connection to Kruskal's Algorithm

While working on this problem, I realized that it is closely related to
Kruskal's algorithm for Minimum Spanning Trees.

Kruskal's algorithm works by:

1. generating all possible edges;
2. sorting the edges from shortest to longest;
3. using a Disjoint Set Union structure to connect components;
4. skipping edges whose endpoints are already in the same component.

Part 1 behaves like a partial version of this idea: it processes only the first
`1000` closest candidate edges.

Part 2 uses the same idea more directly, continuing until all junction boxes
belong to one connected circuit.

## Idea

The distance between two junction boxes with coordinates:

```text
(x1, y1, z1)
```

and:

```text
(x2, y2, z2)
```

is the three-dimensional Euclidean distance:

```text
sqrt((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)
```

However, the square root is not necessary when comparing distances.

If one squared distance is smaller than another, then the actual distance is
also smaller. Therefore, I compare squared distances instead:

```text
(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2
```

This avoids repeatedly computing square roots.

To keep track of the circuits formed by connected junction boxes, I use a
Disjoint Set Union structure.

The solution uses three main data structures:

1. `Point`

   Represents one junction box and stores its `x`, `y`, and `z` coordinates.

2. `Edge`

   Represents a possible connection between two junction boxes. It stores the
   indices of the two endpoints and their squared distance.

3. `DisjointSet`

   Tracks which junction boxes currently belong to the same circuit. It stores:

   - a `parent` array;
   - a `size` array;
   - a `find` operation with path compression;
   - a `merge` operation using union by size.

## Pseudocode — Disjoint Set

```text
DisjointSet(n)
    allocate parent[0...n-1]
    allocate size[0...n-1]

    for i = 0 to n - 1
        parent[i] = i
        size[i] = 1
```

```text
find(i)
    if parent[i] == i
        return i

    parent[i] = find(parent[i])
    return parent[i]
```

```text
merge(i, j)
    root_i = find(i)
    root_j = find(j)

    if root_i == root_j
        return false

    if size[root_i] < size[root_j]
        swap root_i and root_j

    parent[root_j] = root_i
    size[root_i] = size[root_i] + size[root_j]

    return true
```

## Pseudocode — Edge Generation

```text
getSquaredDistance(a, b)
    dx = a.x - b.x
    dy = a.y - b.y
    dz = a.z - b.z

    return dx^2 + dy^2 + dz^2
```

```text
buildEdges(points)
    edges = empty list
    n = points.length

    for i = 0 to n - 2
        for j = i + 1 to n - 1
            dist = getSquaredDistance(points[i], points[j])
            append Edge(i, j, dist) to edges

    sort edges by increasing squared distance

    return edges
```

## Pseudocode — Part 1

```text
solvePart1(points, edges)
    n = points.length

    dsu = DisjointSet(n)

    limit = min(1000, edges.length)

    for k = 0 to limit - 1
        dsu.merge(edges[k].a, edges[k].b)

    circuitSizes = empty list

    for i = 0 to n - 1
        if dsu.parent[i] == i
            append dsu.size[i] to circuitSizes

    sort circuitSizes in decreasing order

    return circuitSizes[0] *
           circuitSizes[1] *
           circuitSizes[2]
```

## Complexity

Let `N` be the number of junction boxes.

The number of unique edges is:

```text
E = N(N - 1) / 2
```

Generating all edges costs:

```text
Θ(N^2)
```

Sorting all edges costs:

```text
Θ(E log E)
```

Since `E = Θ(N^2)`, this becomes:

```text
Θ(N^2 log N)
```

Processing the first `1000` candidate edges uses Disjoint Set operations. With
path compression and union by size, each operation is almost constant time,
usually written as:

```text
O(α(N))
```

where `α` is the inverse Ackermann function.

The total running time is dominated by sorting the edge list:

```text
Θ(N^2 log N)
```

The extra space complexity is dominated by the edge list:

```text
Θ(N^2)
```

---

## Part 2

In Part 2, we continue connecting junction boxes until all of them belong to one
single circuit.

This is equivalent to running Kruskal's algorithm until exactly `N - 1`
successful merges have been performed.

The final successful connection is important: once this edge is added, all
junction boxes belong to the same connected component.

The answer is obtained by multiplying the `x` coordinates of the two junction
boxes connected by that final edge.

## Idea

Part 2 reuses the sorted edge list from Part 1.

At the beginning, every junction box is its own separate circuit. Therefore,
there are `N` independent circuits.

Every successful merge joins two different circuits, reducing the number of
circuits by `1`.

So, after exactly:

```text
N - 1
```

successful merges, all junction boxes must belong to one single circuit.

The algorithm scans the sorted edge list from shortest to longest and performs
a Disjoint Set merge for each edge.

Whenever a merge is successful, the counter `successfulMerges` is incremented.

When `successfulMerges == N - 1`, the current edge is the final edge needed to
connect the whole structure.

## Pseudocode — Part 2

```text
solvePart2(points, edges)
    n = points.length

    if n < 2
        return 0

    dsu = DisjointSet(n)
    successfulMerges = 0

    for each edge in edges
        if dsu.merge(edge.a, edge.b) == true
            successfulMerges = successfulMerges + 1

            if successfulMerges == n - 1
                x1 = points[edge.a].x
                x2 = points[edge.b].x

                return x1 * x2

    return 0
```

## Optimization — Precomputing Edges

Generating and sorting all pairwise edges is the most expensive part of the
program.

Because the junction boxes do not change between Part 1 and Part 2, I generate
and sort the full `allEdges` list once after reading the input.

Both `solvePart1()` and `solvePart2()` then reuse the same sorted edge list.

This keeps the expensive:

```text
Θ(N^2 log N)
```

work out of the individual solver methods.

## Complexity

Let:

- `N` be the number of junction boxes;
- `E = N(N - 1) / 2` be the number of possible edges.

Generating all edges costs:

```text
Θ(N^2)
```

Sorting all edges costs:

```text
Θ(E log E) = Θ(N^2 log N)
```

Part 1 processes at most `1000` candidate edges, plus a scan of the Disjoint Set
arrays to collect circuit sizes.

Part 2 may process edges until the graph becomes fully connected. In the worst
case, this can require scanning many edges, so the DSU phase is bounded by:

```text
O(E α(N))
```

The overall running time is still dominated by sorting the edge list:

```text
Θ(N^2 log N)
```

The extra space complexity is:

```text
Θ(N^2)
```

because all pairwise edges are stored.

## Files

- `main.cpp` — reads the input and prints both answers
- `day08.h` — declares `Point`, `Edge`, `DisjointSet`, and `Playground`
- `day08.cpp` — parses the input, builds the edge list, and solves both parts

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day08.cpp -o day08
```

## Run

```bash
./day08
```

The program expects the puzzle input in a file named `input.txt`.
