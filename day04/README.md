# Advent of Code 2025 — Day 4: Printing Department

## Part 1

In Part 1, we are dealing with a grid of characters containing either `.` or
`@`.

- `.` represents an empty cell.
- `@` represents a roll of paper.

We want to count how many paper rolls have fewer than 4 other paper rolls in
the 8 adjacent positions around them.

For a cell `(i, j)`, the 8 adjacent positions are:

```text
(i-1,j-1)   (i-1,j)   (i-1,j+1)
     ↖          ↑          ↗
(i,j-1)  ←   (i,j)   →  (i,j+1)
     ↙          ↓          ↘
(i+1,j-1)   (i+1,j)   (i+1,j+1)
```

This means that for every cell containing `@`, we check the 8 surrounding
positions. If fewer than 4 of those positions also contain `@`, then that roll
is counted.

Empty cells are skipped.

## Idea

I used two common tricks for grid problems.

### 1. Direction arrays

Instead of writing 8 separate checks manually, I used two arrays representing
the row and column changes for the 8 neighboring positions:

```text
dr = { 1,  1,  1,  0,  0, -1, -1, -1}
dc = {-1,  0,  1, -1,  1, -1,  0,  1}
```

Then, for any cell `(i, j)`, the `k`-th neighbor is:

```text
(i + dr[k], j + dc[k])
```

### 2. Padding

To avoid boundary checks, I store the input grid with an extra border of `.`
characters around it.

So if the original grid has dimensions `N × M`, the stored grid has dimensions:

```text
(N + 2) × (M + 2)
```

The real grid is then located between rows `1` and `N`, and columns `1` and `M`.

This lets me safely check all 8 neighbors of every real cell without worrying
about going out of bounds.

## Pseudocode

```text
countAccessibleRolls(G)
    total = 0

    for i = 1 to N
        for j = 1 to M
            if G[i][j] == '@'
                count = 0

                for k = 0 to 7
                    nr = i + dr[k]
                    nc = j + dc[k]

                    if G[nr][nc] == '@'
                        count = count + 1

                if count < 4
                    total = total + 1

    return total
```

## Complexity

Let:

- `N` be the number of rows in the original grid;
- `M` be the number of columns in the original grid.

For each cell, we perform at most 8 neighbor checks, which is a constant.

Therefore:

- Time complexity: `Θ(N × M)`
- Auxiliary space complexity for Part 1 itself: `Θ(1)`

The program stores the padded grid, which has size:

```text
(N + 2)(M + 2) = NM + 2N + 2M + 4
```

So the stored grid uses `Θ(N × M)` space.

## Part 2

In Part 2, we want to maximize the number of paper rolls that can be removed.

A roll can be removed if it has fewer than 4 neighboring paper rolls. After a
roll is removed, the neighboring rolls may lose one adjacent roll, which can make
some of them newly removable. Therefore, the process can cascade.

This reminded me of Breadth-First Search on a grid. In BFS, we usually start
from one source vertex, put it in a FIFO queue, and then visit its neighbors
level by level. Here, instead of starting from one source, we start from all
paper rolls that are initially removable.

So the idea is similar to a multi-source BFS on a 2D grid:

- each cell is like a vertex;
- the 8 adjacent cells are like its neighbors;
- all initially removable rolls are inserted into the queue at the start;
- removing a roll can make neighboring rolls newly removable;
- those newly removable rolls are then added to the queue.

Technically, this is not BFS for shortest paths. It is better described as a
queue-based cascade or worklist algorithm inspired by multi-source BFS.

## Idea

I use a FIFO queue `Q` to store the coordinates of rolls that can potentially be
removed.

First, I scan the whole grid and enqueue every `@` cell with fewer than 4
neighboring `@` cells.

Then, while the queue is not empty:

1. I pop one coordinate from the queue.
2. If that cell has already been removed, I skip it.
3. Otherwise, I mark it as removed with `x` and increment the answer.
4. I check its 8 neighbors.
5. Any neighboring `@` cell that now has fewer than 4 neighboring `@` cells is
   pushed into the queue.

This works because only the neighbors of a removed roll can have their adjacent
roll count changed.

## Pseudocode

```text
countAllRemovableRolls(G)
    copy = G
    totalRemoved = 0
    Q = empty queue

    for i = 1 to N
        for j = 1 to M
            if copy[i][j] == '@' and countNeighbors(copy, i, j) < 4
                Q.push((i, j))

    while Q is not empty
        (r, c) = Q.pop()

        if copy[r][c] != '@'
            continue

        copy[r][c] = 'x'
        totalRemoved = totalRemoved + 1

        for k = 0 to 7
            nr = r + dr[k]
            nc = c + dc[k]

            if copy[nr][nc] == '@'
                if countNeighbors(copy, nr, nc) < 4
                    Q.push((nr, nc))

    return totalRemoved
```

## Complexity

As in Part 1, let:

- `N` be the number of rows in the original grid;
- `M` be the number of columns in the original grid.

The initial scan checks every cell once, so it costs:

```text
Θ(N × M)
```

During the queue phase, every removed cell causes at most 8 neighboring cells to
be rechecked. Each recheck counts at most 8 neighbors, so this is still a
constant amount of work per queue operation.

A cell may be pushed more than once before it is removed, but this can only happen 
because of one of its 8 neighboring cells being removed. 
Therefore, each cell can only cause a constant amount of duplicate queue work.

- Time complexity: `Θ(N × M)`
- Extra space complexity: `Θ(N × M)`

The extra space comes from the copy of the grid and from the queue, which can
contain a linear number of cells in the worst case.

## Files

- `main.cpp` — reads the input file and prints both answers
- `day04.h` — declarations for `Grid` and `Forklift`
- `day04.cpp` — input parsing and solution logic

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day04.cpp -o day04
```

## Run

```bash
./day04
```

The program expects the puzzle input in a file named `input.txt`.
