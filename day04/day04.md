# Advent of Code 2025 — Day 4: Printing Department

# Part 1

We are dealing with a grid of characters containing either the symbol `'.'` or `'@'`.

The cells are either empty (`'.'`) or contain a roll of paper (`'@'`).

We want to count how many cells containing a roll of paper have fewer than 4 rolls of paper in their 8 adjacent positions.

I was able to understand this visually by drawing something like this for a cell `(i, j)`:

```text
(i-1,j-1)   (i-1,j)   (i-1,j+1)
     ↖          ↑          ↗
(i,j-1)    ←  (i,j)  →    (i,j+1)
     ↙          ↓          ↘
(i+1,j-1)   (i+1,j)   (i+1,j+1)
```

This means that, for any given cell `(i, j)` containing a roll of paper, we must check the 8 adjacent positions.

If the cell `(i, j)` does not contain a roll of paper, we simply skip it.

## Idea

I had previously worked on grid-type problems and learned two tricks that proved helpful.

### 1. Directional arrays

I can use two arrays to help scan the cells adjacent to each cell `(i, j)` in the grid `G`:

```text
dr = {-1, -1, -1, 0, 0, 1, 1, 1}
dc = {-1,  0,  1, -1, 1, -1, 0, 1}
```

I can then simply scan:

```text
G[i + dr[k], j + dc[k]]
```

for `k = 0` to `7` to check all 8 adjacent positions.

### 2. Padding

To avoid having to perform out-of-bounds checks, I can use a grid with an extra border around the original one.

If the original grid has `n` rows and `m` columns, the padded grid has dimensions:

```text
(n + 2) × (m + 2)
```

The first and last rows, as well as the first and last columns, are extra and filled entirely with `'.'`.

This works for any rectangular grid, as long as all input rows have the same length.

In this case, we have a `139 × 139` grid, so when I read each line from the input, I can add the padding directly. I then only work with the actual grid cells, meaning rows `i = 1` to `139` and columns `j = 1` to `139`, inclusive.

## Pseudocode

```text
countAccessibleCells(G)

    // Dimensions of the unpadded grid
    n = G.rows
    m = G.columns

    // Allocate a padded grid G* of size
    // (n + 2) × (m + 2), filled with '.'
    allocate G*[0..n+1, 0..m+1] filled with '.'

    // Copy the original grid into the centre of G*
    for i = 1 to n
        for j = 1 to m
            G*[i, j] = G[i - 1, j - 1]

    totalCount = 0

    // Directional arrays for the 8 adjacent positions
    dr = {-1, -1, -1, 0, 0, 1, 1, 1}
    dc = {-1, 0, 1, -1, 1, -1, 0, 1}

    // Scan only the valid inner cells
    for i = 1 to n
        for j = 1 to m
            if G*[i, j] == '@'

                // Count adjacent '@' symbols
                count = 0

                for k = 0 to 7
                    if G*[i + dr[k], j + dc[k]] == '@'
                        count = count + 1

                // Evaluate the condition
                if count < 4
                    totalCount = totalCount + 1

    return totalCount
```

## Complexity

Let `N` be the number of rows in the grid and `M` be the number of columns.

The outer loops examine exactly `N × M` cells. For each cell containing `'@'`, the inner loop performs exactly 8 constant-time comparisons.

Therefore, the time complexity is:

```text
T(N, M) = Θ(N × M)
```

The scanning algorithm itself uses `Θ(1)` auxiliary space beyond the grid.

The padded grid has dimensions:

```text
(N + 2) × (M + 2)
```

Expanding this expression gives:

```text
NM + 2N + 2M + 4
```

Therefore, the total space required to store the padded grid is:

```text
Θ(N × M)
```

The padding itself adds only `Θ(N + M)` cells compared with the original grid.

# Part 2

In Part 2, we are told that we want to maximize the number of paper rolls that can be removed.

To do this, we can remove any paper roll that is accessible to the forklift, marking it as `'x'` to indicate that it has been removed.

After a removal, more paper rolls may become newly accessible and therefore removable. The process repeats until no removable paper rolls remain.

We want to count the total number of paper rolls that can be removed.

This reminded me of a Breadth-First Search traversal of a graph, which traverses a graph starting from a given root vertex and visits its immediate adjacent vertices before moving deeper.

This is normally done using a FIFO queue.

In our context, we are operating on a 2D grid, so every cell can be considered a vertex, while its 8 adjacent cells represent its neighbours.

Instead of expanding from a single starting point, we begin with all the cells found by the Part 1 condition.

This is therefore similar to a multi-source BFS, although it is more precisely a queue-based cascade or worklist algorithm, since we are not computing distances or shortest paths.

## Idea

We can use this queue-based cascade algorithm for Part 2.

We use a FIFO queue `Q` to store the coordinates of cells that can be removed.

The queue is initially filled with all coordinates `(i, j)` of the cells found using the Part 1 condition.

After this initial phase, we use a counter `totalRemoved` and operate on a copy of the grid so that we do not modify the original data.

We then perform the cascading removals.

As long as `Q` contains coordinates, we:

1. Pop a coordinate from the front.
2. Verify that it has not already been removed, to avoid duplicate processing.
3. Mark it as `'x'` and increment `totalRemoved`.
4. Explore its 8 adjacent positions.
5. If one of these neighbours contains `'@'`, it may have just become accessible because we removed a paper roll next to it.
6. Count the neighbouring `'@'` cells around that roll.
7. If the count is fewer than 4, push that neighbour's coordinates into the back of `Q`.

A cell may be inserted into the queue more than once before it is processed. The check performed when it is removed from the queue ensures that an already removed roll is skipped.

The process stops when no more cascading removals are possible.

## Pseudocode

Assume that `G[0..n+1, 0..m+1]` is the padded grid, meaning that the effective grid is contained in:

```text
G[1..n, 1..m]
```

```text
solvePart2(G)

    // Create a local copy of the grid
    copy = G

    rows = copy.height
    cols = copy.width
    totalRemoved = 0

    allocate Q // Initially empty

    // Directional arrays
    dr = {1, 1, 1, 0, 0, -1, -1, -1}
    dc = {-1, 0, 1, -1, 1, -1, 0, 1}

    // Populate the initial queue
    for i = 1 to rows - 2
        for j = 1 to cols - 2
            if copy[i, j] == '@'
                count = 0

                for k = 0 to 7
                    if copy[i + dr[k], j + dc[k]] == '@'
                        count = count + 1

                if count < 4
                    Q.push((i, j))

    // Process the queue
    while |Q| > 0
        (r, c) = Q.pop()

        // Skip if already processed
        if copy[r, c] != '@'
            continue

        // Remove the paper roll
        copy[r, c] = 'x'
        totalRemoved = totalRemoved + 1

        // Check adjacent positions to see whether
        // they are now removable
        for k = 0 to 7
            nr = r + dr[k]
            nc = c + dc[k]

            if copy[nr, nc] == '@'
                neighbourCount = 0

                for nk = 0 to 7
                    if copy[nr + dr[nk], nc + dc[nk]] == '@'
                        neighbourCount = neighbourCount + 1

                if neighbourCount < 4
                    Q.push((nr, nc))

    return totalRemoved
```

## Complexity

As in Part 1, let `N` and `M` be the number of rows and columns in the grid.

The initial scan takes:

```text
Θ(N × M)
```

because it examines every cell exactly once and performs at most 8 neighbour checks for each cell.

During the second phase, each removed cell causes at most 8 neighbouring cells to be reconsidered. For each neighbour, we check its 8 adjacent positions.

Therefore, each removal causes at most:

```text
8 × 8 = 64
```

comparisons, which is constant work.

A cell can be added to the queue more than once before being removed, but it can only be reconsidered after one of its at most 8 neighbours is removed. Therefore, the number of duplicate queue entries per cell is bounded by a constant.

The entire queue-processing phase is consequently:

```text
O(N × M)
```

Since the initial grid scan already requires `Θ(N × M)` time, the total time complexity is:

```text
T(N, M) = Θ(N × M)
```

Unlike Part 1, the algorithm uses `Θ(N × M)` additional space for the copy of the grid.

The queue may also contain `O(N × M)` coordinates in the worst case.

Therefore, including the copied padded grid and the queue, the total auxiliary space complexity is:

```text
Θ(N × M)
```
