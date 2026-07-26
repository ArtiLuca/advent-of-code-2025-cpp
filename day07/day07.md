# Advent of Code 2025 — Day 7: Teleporter Hub

# Part 1

In Part 1, we are dealing with a tachyon manifold.

We are trying to fix a broken teleporter, specifically an issue with one of its tachyon manifolds, for which we have a manual to help us solve the problem: our puzzle input.

The tachyon manifold manual is represented as a 2D grid containing `'S'`, `'.'`, or `'^'`.

* `'S'` represents the entry point of the tachyon beam.
* `'.'` represents empty space.
* `'^'` represents a splitter.

A tachyon beam enters the manifold at `'S'` and propagates strictly downwards, one row at a time.

Empty space allows the beam to pass freely straight down to the next row. When a beam encounters a splitter, it is stopped.

Two new beams are immediately created on the same row: one to its immediate left and one to its immediate right.

We want to calculate the total number of times the beams are split before all active beams either exit the boundaries of the manifold or terminate.

## Idea

This reminded me of Day 4, where we used a queue-based cascade or worklist algorithm.

However, because the tachyon beams propagate strictly downwards between rows, we can optimize the tracking structure to evaluate the grid layer by layer, meaning row by row.

When a splitter is encountered, it creates lateral beams that might hit neighbouring splitters. This can trigger a horizontal chain reaction on the same layer before any beams propagate down to the next row.

To solve this without simulating an exponential number of independent beam paths, I used two tracking mechanisms.

### 1. Active Columns Set

Instead of tracking distinct paths, I maintain a unique set of column indices representing beams that are actively propagating downwards.

We advance row by row. When moving from row `r - 1` to row `r`, the previous row's active columns give us the initial entry columns for the current row.

### 2. Row Queue and Visited Tracker

For the current row `r`, all active entry columns are transferred into a FIFO queue.

I also maintain a visited set to prevent the same column from being processed more than once on the current row.

While the queue is not empty:

1. I pop a column `c` from the queue.
2. If `c` is out of bounds or already in the visited set, I skip it.
3. Otherwise, I mark it as visited.
4. If the cell is a splitter `'^'` and it has not already been counted, I increment the total number of splits and push `c - 1` and `c + 1` into the queue to continue the lateral chain reaction.
5. If the cell is `'.'`, the beam passes through, and no lateral neighbours are added.

Once the queue is empty, the row-level visited set contains every column reached by a tachyon beam on the current row.

By filtering out the splitter cells, the remaining columns become the initial active set for row `r + 1`. Using sets also ensures that overlapping paths do not cause the same column to propagate downwards multiple times.

## Pseudocode

```text
// G contains the 2D manifold grid
simulateTachyonManifold(G)
    R = G.rows
    C = G.columns

    startRow = -1
    startCol = -1
    totalSplits = 0

    // Find the entry point
    for i = 0 to R - 1
        for j = 0 to C - 1
            if G[i][j] == 'S'
                startRow = i
                startCol = j
                break

        if startRow != -1
            break

    if startRow == -1
        return 0

    activeCols = empty set
    activeCols.insert(startCol)

    // Process the manifold row by row
    for r = startRow + 1 to R - 1
        if activeCols is empty
            break

        Q = empty queue
        visited = empty set
        splittersEncountered = empty set

        for each c in activeCols
            Q.push(c)

        // Process all same-row cascades
        while Q is not empty
            c = Q.pop()

            if c < 0 or c >= C or visited.contains(c)
                continue

            visited.insert(c)

            if G[r][c] == '^'
                if not splittersEncountered.contains(c)
                    splittersEncountered.insert(c)
                    totalSplits = totalSplits + 1

                    Q.push(c - 1)
                    Q.push(c + 1)

        // Only non-splitter cells continue downwards
        nextActiveCols = empty set

        for each c in visited
            if G[r][c] != '^'
                nextActiveCols.insert(c)

        activeCols = nextActiveCols

    return totalSplits
```

## Complexity

Let:

* `R` be the number of rows in the manifold grid;
* `C` be the number of columns in the manifold grid.

Finding the starting element `'S'` requires checking at most all the cells in the grid, giving a worst-case cost of:

```text
Θ(R × C)
```

The outer loop then moves down through at most `R` rows.

For each row, every valid column is processed at most once because of the visited set. The queue may contain duplicate entries, but each splitter can add only its two immediate neighbours, so the number of queue entries per row remains `O(C)`.

The implementation uses ordered `std::set` structures. Each lookup or insertion therefore costs `O(log C)` in the worst case.

The overall worst-case time complexity is:

```text
O(R × C log C)
```

The extra space complexity is:

```text
Θ(C)
```

The active-column set, queue, visited set, splitter set, and next-row set can each contain at most `C` column indices at any given time.

# Part 2

In Part 2, we discover that the teleporter relies on a quantum tachyon manifold.

Instead of spawning multiple physical beams at a splitter, a single tachyon particle splits time itself, taking both the left and right paths in alternate timelines.

We want to calculate the total number of distinct timelines the particle ends up in after completing its journey through the manifold.

Because timelines branch at every splitter, the number of possible paths can grow exponentially.

## Idea

Simulating this using the queue-based cascade from Part 1 could result in an exponential explosion of individual active paths, quickly exhausting both memory and computation time.

Because of this, I decided to use a bottom-up Dynamic Programming approach.

Instead of tracking where each individual timeline goes, which branches outwards, we count how many timelines reach each position, allowing different paths to converge into the same state.

We define a state $t(i,j)$ representing the number of timelines occupying cell $(i,j)$ and moving downwards.

Let:

* $R$ be the number of rows;
* $C$ be the number of columns;
* $0 \le i < R$;
* $0 \le j < C$.

Let $\operatorname{split}(i,j)$ be true exactly when `G[i][j] == '^'`.

Since timelines immediately move laterally when they encounter a splitter, the number of timelines continuing straight down through a splitter cell is always zero.

For an empty cell $(i,j)$, timelines can arrive from three possible locations:

1. Straight down from column $j$ in the previous row, provided that the current path was not stopped by a splitter.
2. From column $j-1$ in the previous row, if the cell immediately to the left in the current row is a splitter redirecting timelines to the right.
3. From column $j+1$ in the previous row, if the cell immediately to the right in the current row is a splitter redirecting timelines to the left.

If the entry point is located at $(r_S,c_S)$, the base case is:

$$
t(r_S,c_S)=1.
$$

For the later rows, the recurrence is:

$$
t(i,j)=
\begin{cases}
0,
& \text{if } \operatorname{split}(i,j), [4pt]
t_{\mathrm{up}}(i,j)
+t_{\mathrm{left}}(i,j)
+t_{\mathrm{right}}(i,j),
& \text{if cell } (i,j) \text{ is empty}.
\end{cases}
$$

The contribution from directly above is:

$$
t_{\mathrm{up}}(i,j)=
\begin{cases}
t(i-1,j),
& \text{if } \neg\operatorname{split}(i-1,j), [4pt]
0,
& \text{otherwise}.
\end{cases}
$$

The contribution redirected from the splitter on the left is:

$$
t_{\mathrm{left}}(i,j)=
\begin{cases}
t(i-1,j-1),
& \text{if } j>0
\text{ and } \operatorname{split}(i,j-1), [4pt]
0,
& \text{otherwise}.
\end{cases}
$$

The contribution redirected from the splitter on the right is:

$$
t_{\mathrm{right}}(i,j)=
\begin{cases}
t(i-1,j+1),
& \text{if } j<C-1
\text{ and } \operatorname{split}(i,j+1), [4pt]
0,
& \text{otherwise}.
\end{cases}
$$

The important distinction is that the timeline counts come from row $i-1$, while the splitters causing the lateral redirections are located in the current row $i$.

To optimize memory, we do not need to keep the entire 2D DP table.

Because row $i$ only depends on row $i-1$, we can keep the space requirement to a minimum by storing only the timeline counts for the immediately preceding row and the current row.

## Pseudocode

```text
// G contains the 2D manifold grid
solvePart2(G)
    R = G.rows
    C = G.columns
    startRow = -1

    // Track timeline counts for the previous row
    allocate prevRow[0..C-1] filled with 0

    // Locate the entry point and initialize the base case
    for r = 0 to R - 1
        for c = 0 to C - 1
            if G[r][c] == 'S'
                prevRow[c] = 1
                startRow = r
                break

        if startRow != -1
            break

    if startRow == -1
        return 0

    // Process the grid layer by layer
    for i = startRow + 1 to R - 1
        allocate currRow[0..C-1] filled with 0

        for j = 0 to C - 1
            if G[i][j] == '^'
                currRow[j] = 0

            else if G[i][j] == '.'
                sum = 0

                // Timeline arriving directly from above
                if G[i - 1][j] != '^'
                    sum = sum + prevRow[j]

                // Timeline redirected right by a splitter
                // immediately to the left on the current row
                if j > 0 and G[i][j - 1] == '^'
                    sum = sum + prevRow[j - 1]

                // Timeline redirected left by a splitter
                // immediately to the right on the current row
                if j < C - 1 and G[i][j + 1] == '^'
                    sum = sum + prevRow[j + 1]

                currRow[j] = sum

        // Advance the DP state
        prevRow = currRow

    // Sum the timelines that reach the bottom row
    totalTimelines = 0

    for j = 0 to C - 1
        totalTimelines = totalTimelines + prevRow[j]

    return totalTimelines
```

## Complexity

Let:

* `R` be the number of rows in the manifold grid;
* `C` be the number of columns in the manifold grid.

We first scan the grid to find the starting point, which takes at most:

```text
Θ(R × C)
```

operations.

We then iterate through the grid once from top to bottom. For every cell, we perform at most three constant-time array lookups and additions.

Therefore, the time complexity is:

```text
Θ(R × C)
```

The extra space complexity is:

```text
Θ(C)
```

We maintain only two one-dimensional arrays, `prevRow` and `currRow`, each of length `C`. This avoids the `Θ(R × C)` space required by a complete 2D Dynamic Programming table.
