# Advent of Code 2025 — Day 7: Teleporter Hub

## Part 1

In Part 1, we are dealing with a tachyon manifold.

The input is a two-dimensional grid representing the manual for a broken
teleporter. Each cell can contain one of three symbols:

- `S` — the entry point of the tachyon beam;
- `.` — empty space;
- `^` — a splitter.

The beam starts at `S` and moves strictly downward, one row at a time.

Empty space allows the beam to continue downward. When a beam encounters a
splitter, that beam stops and two new beams are created on the same row: one to
the immediate left and one to the immediate right.

Those new beams may themselves encounter neighboring splitters, creating a
same-row chain reaction.

The goal is to count how many splitters are triggered before all beams either
leave the manifold or terminate.

## Idea

This reminded me of Day 4, where I used a queue-based cascade / worklist
algorithm.

The important difference is that here the beam always progresses downward
between rows, but splitters can create horizontal activity inside the current
row.

To avoid simulating an exponential number of independent beam paths, I track
only the set of columns that currently contain active downward beams.

The algorithm works row by row.

For each row:

1. the active columns from the previous row are inserted into a queue;
2. the queue processes all same-row activity;
3. when a splitter is found, the split counter increases and the neighboring
   columns are pushed into the queue;
4. once the row is finished, only the non-splitter cells reached by beams are
   allowed to continue downward to the next row.

A `visited` set is used for each row so that the same column is not processed
more than once during the same row-level cascade.

## Pseudocode

```text
simulateManifold(G)
    R = number of rows in G
    C = number of columns in G

    startRow = -1
    startCol = -1
    totalSplits = 0

    for r = 0 to R - 1
        for c = 0 to C - 1
            if G[r][c] == 'S'
                startRow = r
                startCol = c
                break

        if startRow != -1
            break

    if startRow == -1
        return 0

    activeCols = empty set
    insert startCol into activeCols

    for r = startRow + 1 to R - 1
        if activeCols is empty
            break

        Q = empty queue
        visited = empty set

        for each c in activeCols
            push c into Q

        while Q is not empty
            c = Q.pop()

            if c < 0 or c >= C
                continue

            if c is already in visited
                continue

            insert c into visited

            if G[r][c] == '^'
                totalSplits = totalSplits + 1

                push c - 1 into Q
                push c + 1 into Q

        nextActiveCols = empty set

        for each c in visited
            if G[r][c] != '^'
                insert c into nextActiveCols

        activeCols = nextActiveCols

    return totalSplits
```

## Complexity

Let:

- `R` be the number of rows in the manifold;
- `C` be the number of columns in the manifold.

Finding the start cell `S` takes at most:

```text
Θ(RC)
```

The simulation then processes the grid row by row. For each row, every column
can be inserted into the row-level visited structure at most once.

Because the implementation uses `std::set`, insertions and lookups cost
`Θ(log C)`.

Therefore, the total time complexity is:

```text
Θ(RC log C)
```

The extra space complexity is:

```text
Θ(C)
```

because the queue and row-level sets store at most `C` column indices at a time.

---

## Part 2

In Part 2, the teleporter uses a quantum tachyon manifold.

Instead of only counting how many splitters are triggered, we now want to count
how many distinct timelines exist after the tachyon particle finishes moving
through the manifold.

Each splitter branches the current timeline into two possible directions.

## Idea

A direct simulation of all individual timelines could grow exponentially,
because every splitter may branch one path into two.

Instead, I use dynamic programming.

The key idea is to count how many timelines reach each cell, rather than storing
each timeline separately.

Let:

```text
t(i, j)
```

be the number of timelines reaching row `i`, column `j`, and moving downward.

The start cell contributes one timeline.

A splitter cell does not keep a timeline falling straight downward, because any
timeline entering it is redirected left and right.

For an empty cell, timelines may arrive from:

1. the cell directly above, if the above cell was not a splitter;
2. the column to the left, if that cell in the current row is a splitter
   redirecting right;
3. the column to the right, if that cell in the current row is a splitter
   redirecting left.

The implementation only needs the previous row of DP values, because row `i`
depends on row `i - 1`.

## Recurrence

For the start cell:

```text
t(startRow, startCol) = 1
```

For each later cell `(i, j)`:

```text
t(i, j) = 0                         if G[i][j] == '^'

t(i, j) = up + fromLeft + fromRight  if G[i][j] == '.'
```

where:

```text
up =
    t(i - 1, j)    if G[i - 1][j] != '^'
    0              otherwise
```

```text
fromLeft =
    t(i - 1, j - 1)    if j > 0 and G[i][j - 1] == '^'
    0                  otherwise
```

```text
fromRight =
    t(i - 1, j + 1)    if j < C - 1 and G[i][j + 1] == '^'
    0                  otherwise
```

## Pseudocode

```text
solvePart2(G)
    R = number of rows in G
    C = number of columns in G

    startRow = -1

    prevRow = array of C zeroes

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

    for i = startRow + 1 to R - 1
        currRow = array of C zeroes

        for j = 0 to C - 1
            if G[i][j] == '^'
                currRow[j] = 0

            else if G[i][j] == '.'
                sum = 0

                if G[i - 1][j] != '^'
                    sum = sum + prevRow[j]

                if j > 0 and G[i][j - 1] == '^'
                    sum = sum + prevRow[j - 1]

                if j < C - 1 and G[i][j + 1] == '^'
                    sum = sum + prevRow[j + 1]

                currRow[j] = sum

        prevRow = currRow

    totalTimelines = 0

    for j = 0 to C - 1
        totalTimelines = totalTimelines + prevRow[j]

    return totalTimelines
```

## Complexity

Let:

- `R` be the number of rows in the manifold;
- `C` be the number of columns in the manifold.

Finding the start cell takes:

```text
Θ(RC)
```

The dynamic programming pass visits every cell once and performs only a constant
number of checks per cell.

Therefore, the time complexity is:

```text
Θ(RC)
```

The extra space complexity is:

```text
Θ(C)
```

because only two one-dimensional arrays of length `C` are needed: one for the
previous row and one for the current row.

## Files

- `main.cpp` — reads the input and prints both answers
- `day07.h` — declares the tachyon manifold structure, constants, and solver interface
- `day07.cpp` — implements input parsing, Part 1 simulation, and Part 2 dynamic programming

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day07.cpp -o day07
```

## Run

```bash
./day07
```

The program expects the puzzle input in a file named `input.txt`.
