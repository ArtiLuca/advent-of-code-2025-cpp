# Advent of Code 2025 — Day 8: Playground

## Part 1

In Part 1, we are dealing with junction boxes, which are given to us as a long list of 3D coordinates `(x, y, z)`.

Our main goal is to connect pairs of junction boxes that are as close together as possible.

If we have `N` junction boxes, we consider every possible pair in order of increasing distance.

Initially, every junction box belongs to its own separate circuit. When two junction boxes from different circuits are connected, their circuits merge into a single larger circuit. If both junction boxes already belong to the same circuit, nothing changes.

We continue this process until we have considered the first 1000 pairs of junction boxes in the sorted list.

After that, we multiply the sizes of the three largest circuits to obtain our result.

## Note

It is clear that we want to work with a sorted list of possible connections, but I was initially unsure about how to implement this efficiently.

At first, I thought about using a priority queue with a min-heap to avoid quadratic complexity. However, after some thought, I noticed that we still need to generate or otherwise consider a quadratic number of possible pairs, and keeping the priority queue correctly updated could prove tricky.

Therefore, I decided to rely on an overloaded version of `std::sort`. For a puzzle input of this size, this should work well enough.

Given `N` junction boxes, the number of unique pairs is:

$$
\frac{N(N-1)}{2}.
$$

For example, if `N = 1000`, then the number of possible pairs is:

$$
\frac{1000 \cdot 999}{2}=499500.
$$

These are possible pairs or edges, rather than the number of comparisons performed by the sorting algorithm.

There are probably more advanced ways to approach the problem, such as spatial hashing or k-dimensional trees. For Part 1 alone, a bounded max-priority queue could also retain only the 1000 shortest candidate edges while all pairs are generated.

However, these approaches are still new to me, so I decided to first implement the clearer solution using `std::sort`.

### Note: Kruskal's Algorithm and Minimum Spanning Trees

While researching ways to optimize the process of connecting these points, I learned that the problem is closely related to the Minimum Spanning Tree problem.

A Minimum Spanning Tree connects all vertices of a weighted graph without cycles and with the minimum possible total edge weight.

Kruskal's algorithm works by:

1. Sorting all possible edges from shortest to longest.
2. Iterating through the sorted edges.
3. Using a Disjoint Set Union structure to merge different connected components.
4. Skipping edges whose endpoints are already in the same component, preventing cycles.

Part 1 follows the same sorted-edge and DSU approach, but it stops after considering the first 1000 candidate edges. These 1000 edges include edges whose endpoints may already belong to the same circuit.

Part 2 follows Kruskal's algorithm more directly, continuing until exactly `N - 1` edges have been successfully merged and the entire graph has become one connected tree.

## Idea

First, we can optimize the sorting by working only with the squared distance between any two junction boxes.

The distance between junction boxes with coordinates $(x_1,y_1,z_1)$ and $(x_2,y_2,z_2)$ is given by the three-dimensional Euclidean distance formula:

$$
d=
\sqrt{
(x_1-x_2)^2+
(y_1-y_2)^2+
(z_1-z_2)^2
}.
$$

However, to avoid repeated square-root operations, we can work with the squared distance:

$$
d^2=
(x_1-x_2)^2+
(y_1-y_2)^2+
(z_1-z_2)^2.
$$

Because the square-root function is strictly increasing for non-negative values, sorting by squared distance produces the same order as sorting by the actual Euclidean distance.

The main difficulty is keeping track of the circuits created when connecting the junction boxes. We need to know which circuit each junction box belongs to and the size of each circuit.

We can make this easier by modelling three data structures—`Point`, `Edge`, and `DisjointSet`—and implementing an algorithm similar to Kruskal's algorithm.

### 1. Point

We model each junction box read from the input as a `Point` containing its three-dimensional coordinates `(x, y, z)`.

### 2. Edge

Assuming that the junction boxes are stored in an index-based list, we model each possible connection between two junction boxes as an `Edge`.

Each edge stores:

* the index of its first endpoint;
* the index of its second endpoint;
* their squared distance.

### 3. DisjointSet

We use a Disjoint Set Union structure to keep track of the circuits created as we connect the junction boxes.

The structure operates on the indices of the junction boxes and stores two arrays:

* a `parent` array containing the parent index of each junction box;
* a `size` array containing the size of each circuit root.

Initially, each junction box is the root of its own circuit, and every circuit has size `1`.

To use the Disjoint Set structure efficiently, we implement:

* a constructor;
* a `find` helper to determine which circuit a junction box belongs to;
* a `merge` helper to combine two different circuits.

In particular, we optimize the `find` procedure using path compression, which speeds up future lookups. We also use union by size, attaching the smaller circuit to the root of the larger circuit.

Once these three structures have been implemented, we can solve Part 1 with the following steps:

1. Generate every unique pair of junction boxes and store its endpoint indices and squared distance as an `Edge`.
2. Sort all the edges from shortest to longest using `std::sort`.
3. Initialize a Disjoint Set containing all the junction boxes.
4. Process the first 1000 candidate edges, merging their endpoint circuits whenever they are different.
5. Extract the final circuit sizes from the Disjoint Set.
6. Sort the circuit sizes in decreasing order.
7. Multiply the three largest sizes to obtain the Part 1 result.

## Pseudocode — Data Structures

The main procedures for the `DisjointSet` data structure can be implemented as follows:

```text
DisjointSet(n)
    allocate parent[0..n-1]
    allocate size[0..n-1]

    for i = 0 to n - 1
        parent[i] = i
        size[i] = 1
```

```text
// i is the index of a junction box
find(i)
    if parent[i] == i
        return i

    // Path-compression optimization
    parent[i] = find(parent[i])
    return parent[i]
```

```text
// i and j are indices of two junction boxes
merge(i, j)
    root_i = find(i)
    root_j = find(j)

    // If they already belong to the same circuit,
    // nothing changes
    if root_i == root_j
        return

    // Ensure that root_i represents the larger circuit
    if size[root_i] < size[root_j]
        swap root_i and root_j

    // Merge the smaller circuit into the larger one
    parent[root_j] = root_i
    size[root_i] = size[root_i] + size[root_j]
```

Assuming that `a` and `b` are two `Point` objects, their squared distance can be calculated as follows:

```text
getSquaredDistance(a, b)
    dx = a.x - b.x
    dy = a.y - b.y
    dz = a.z - b.z

    return dx² + dy² + dz²
```

## Pseudocode — Part 1

Assume that `L[0..n-1]` contains all the junction boxes read from the input, stored as `Point` objects.

```text
solvePart1(L)
    n = L.length
    allEdges = empty list

    // Generate all unique pairs of junction boxes
    for i = 0 to n - 2
        for j = i + 1 to n - 1
            squaredDistance =
                getSquaredDistance(L[i], L[j])

            newEdge = new Edge
            newEdge.a = i
            newEdge.b = j
            newEdge.squaredDistance = squaredDistance

            append newEdge to allEdges

    // Sort all candidate edges from shortest to longest
    sort allEdges in ascending order
        by squaredDistance

    // Initialize the Disjoint Set structure
    dsu = DisjointSet(n)

    // Consider the first 1000 candidate edges,
    // or every edge if fewer than 1000 exist
    numberOfConnections =
        min(1000, allEdges.size)

    for k = 0 to numberOfConnections - 1
        dsu.merge(allEdges[k].a, allEdges[k].b)

    // Extract the final circuit sizes
    circuitSizes = empty list

    for i = 0 to n - 1
        // A root is its own parent
        if dsu.parent[i] == i
            append dsu.size[i] to circuitSizes

    // Sort sizes in decreasing order
    sort circuitSizes in decreasing order

    // Multiply the three largest circuit sizes
    solution =
        circuitSizes[0] *
        circuitSizes[1] *
        circuitSizes[2]

    return solution
```

## Complexity — Part 1

Let:

* `N` be the number of junction boxes;
* `E` be the number of possible edges.

The number of unique edges is:

$$
E=\frac{N(N-1)}{2}=\Theta(N^2).
$$

Generating all edges takes:

$$
\Theta(N^2).
$$

Sorting the edge list using a comparison sort takes:

$$
O(E\log E).
$$

Since $E=\Theta(N^2)$:

$$
O(E\log E)=O(N^2\log N).
$$

Processing the first 1000 candidate edges performs at most 1000 Disjoint Set operations. With path compression and union by size, each operation has an amortized cost of:

$$
O(\alpha(N)),
$$

where $\alpha$ is the inverse Ackermann function.

Collecting the circuit sizes requires `Θ(N)` time, while sorting them requires at most:

$$
O(N\log N).
$$

The overall running time is dominated by generating and sorting the edge list:

$$
O(N^2\log N).
$$

The edge list stores `Θ(N²)` edges, while the Disjoint Set arrays and circuit-size list require `Θ(N)` space.

Therefore, the total space complexity is:

$$
\Theta(N^2).
$$

---

## Part 2

In Part 2, we are told that we need to keep connecting junction boxes until they all belong to one large circuit.

Instead of stopping after considering exactly 1000 candidate connections, as in Part 1, we continue processing the sorted edges and track the exact moment when all the junction boxes become unified into one circuit.

Specifically, we want to identify the final edge that completes this unified circuit and multiply the `x` coordinates of its two endpoints to obtain the Part 2 solution.

## Idea

We can adapt the solution used for Part 1.

We continue using the `allEdges` array, which contains every possible connection sorted from shortest to longest distance.

We can also reuse the `DisjointSet` structure. However, we now need to know whether a merge successfully connected two previously separate circuits.

At the beginning, there are `N` independent circuits.

Every successful merge joins two different circuits, reducing the total number of circuits by exactly `1`.

Therefore, starting from `N` independent junction boxes, they become unified into a single circuit after exactly:

$$
N-1
$$

successful merge operations.

We can implement Part 2 with the following steps:

1. Implement an alternative version of the merge procedure, called `mergePart2`, which returns `true` if two different circuits were successfully merged and `false` if the junction boxes already belonged to the same circuit.
2. Loop through the sorted `allEdges` array sequentially.
3. Maintain a counter called `successfulMerges`, incrementing it whenever `mergePart2` returns `true`.
4. Stop when `successfulMerges == N - 1`.
5. Use the current edge's endpoint indices to retrieve the corresponding `Point` structures.
6. Multiply their `x` coordinates to obtain the Part 2 solution.

## Pseudocode — Successful Merge

```text
mergePart2(i, j)
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

## Pseudocode — Part 2

Assume that:

* `L[0..n-1]` contains all the junction boxes as `Point` objects;
* `allEdges` contains all possible pairs sorted from shortest to longest distance.

```text
solvePart2(L, allEdges)
    n = L.length

    if n < 2
        return 0

    // Reinitialize the Disjoint Set structure
    dsu = DisjointSet(n)

    successfulMerges = 0

    // Process all edges from shortest to longest
    for k = 0 to allEdges.size - 1
        edge = allEdges[k]

        if dsu.mergePart2(edge.a, edge.b) == true
            successfulMerges =
                successfulMerges + 1

            // All junction boxes now belong
            // to one circuit
            if successfulMerges == n - 1
                x1 = L[edge.a].x
                x2 = L[edge.b].x

                return x1 * x2

    return 0
```

## Optimization: Precomputing Edges

Calculating the squared distance for all `Θ(N²)` pairs and sorting them is the most computationally expensive part of the program.

Because the junction boxes do not change between Part 1 and Part 2, I optimized the program architecture by generating and sorting the `allEdges` list exactly once at the end of the `readPuzzleInput()` phase.

The sorted edges are stored as a class member, allowing both `solvePart1()` and `solvePart2()` to reuse the same list instead of repeating the expensive edge-generation and sorting steps.

## Complexity

Let:

* `N` be the number of junction boxes;
* $E=N(N-1)/2$ be the number of possible edges.

### Precomputation

Generating all edges takes:

$$
\Theta(N^2).
$$

Sorting them takes:

$$
O(E\log E)=O(N^2\log N).
$$

### Part 1 After Precomputation

Part 1 processes at most 1000 candidate edges, requiring:

$$
O\bigl(\min(1000,E)\alpha(N)\bigr)
$$

time for the Disjoint Set operations.

It also scans the `N` Disjoint Set entries and sorts up to `N` circuit sizes, giving:

$$
O(N\log N)
$$

time overall after precomputation.

### Part 2 After Precomputation

Part 2 stops after `N - 1` successful merges. However, it may also inspect unsuccessful edges whose endpoints already belong to the same circuit.

In the worst case, it may scan all `E` edges.

Therefore, the DSU-processing phase is bounded by:

$$
O(E\alpha(N)).
$$

### Total Time

The complete program is dominated by generating and sorting the edge list:

$$
O(N^2\log N).
$$

### Space Complexity

The `allEdges` list stores every unique connection pair and therefore requires:

$$
\Theta(N^2)
$$

space.

The Disjoint Set structure uses:

$$
\Theta(N)
$$

space for its `parent` and `size` arrays.

Therefore, the total space complexity is:

$$
\Theta(N^2).
$$
