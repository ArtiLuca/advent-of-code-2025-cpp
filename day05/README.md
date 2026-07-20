# Advent of Code 2025 — Day 5: Cafeteria

## Part 1

In Part 1, we are working with product IDs corresponding to certain cafeteria
products or ingredients.

The input first gives a set of inclusive ranges of product IDs that are
considered fresh. Each range is written in the form:

```text
start-end
```

The ranges can overlap.

After the ranges, we are given a list of standalone product IDs to check. The
goal is to count how many of these product IDs fall inside at least one fresh
range.

Product IDs that do not fall inside any fresh range are considered spoiled.

## Idea

A simple approach would be to check every product ID against every range.

If there are `P` product IDs and `N` ranges, this would cost:

```text
O(P × N)
```

To make the search more efficient, I first sort the ranges by their starting
value and then merge any overlapping ranges.

After this preprocessing step, the list of ranges is sorted and disjoint. This
means that for each product ID, I can use binary search to check whether it
belongs to one of the merged ranges.

For a product ID and a middle range, there are only three cases:

1. the ID is inside the middle range;
2. the ID is smaller than the middle range, so I discard the right half;
3. the ID is larger than the middle range, so I discard the left half.

I used an iterative binary search instead of a recursive one because it is
simple and avoids using extra call-stack space.

## Implementation idea

The solution has three main phases.

### Phase 1 — Sort the ranges

The ranges are sorted by their starting value.

In the actual C++ implementation, I use `std::sort` instead of writing my own
sorting algorithm.

### Phase 2 — Merge overlapping ranges

After sorting, overlapping ranges can be collapsed with one linear scan.

For example:

```text
10-20
15-30
```

can be merged into:

```text
10-30
```

This gives a smaller list of disjoint ranges.

### Phase 3 — Binary search

For each standalone product ID, I perform binary search over the merged ranges.

If the ID falls inside one of the ranges, it is counted as fresh.

## Pseudocode

### Sorting and merging ranges

```text
prepareRanges(R)
    sort R by starting value

    M = empty list

    for each range current in R
        if M is empty
            add current to M

        else if current.start > M.last.end
            add current as a new separate range to M

        else
            M.last.end = max(M.last.end, current.end)

    return M
```

### Binary search

```text
binarySearch(M, id)
    left = 0
    right = M.size - 1

    while left <= right
        mid = left + (right - left) / 2

        if id >= M[mid].start and id <= M[mid].end
            return true

        else if id < M[mid].start
            right = mid - 1

        else
            left = mid + 1

    return false
```

### Counting fresh product IDs

```text
countFreshProducts(M, P)
    count = 0

    for each id in P
        if binarySearch(M, id) == true
            count = count + 1

    return count
```

## Complexity

Let:

- `N` be the number of input ranges;
- `P` be the number of standalone product IDs;
- `K` be the number of merged ranges, where `K <= N`.

The preprocessing step has two parts:

1. sorting the ranges costs `Θ(N log N)`;
2. merging overlapping ranges costs `Θ(N)`.

Then, for each of the `P` product IDs, binary search over the merged ranges costs
`O(log K)`, which is bounded by `O(log N)`.

Therefore, the total time complexity is:

```text
Θ(N log N) + Θ(N) + O(P log N)
```

which can be written as:

```text
O((N + P) log N)
```

The extra space used by the merged range list is:

```text
Θ(N)
```

## Part 2

In Part 2, we no longer need to check the standalone product IDs.

Instead, we need to count the total number of unique fresh product IDs covered by
the input ranges.

The important observation is that, after the sorting and merging phase from Part
1, the merged ranges are disjoint. Therefore, no product ID is counted twice.

## Idea

For a single inclusive range:

```text
[start, end]
```

the number of product IDs inside the range is:

```text
end - start + 1
```

So for Part 2, I simply iterate through the merged ranges and add the size of
each range.

## Pseudocode

```text
countAllFreshProducts(M)
    count = 0

    for each range in M
        rangeSize = range.end - range.start + 1
        count = count + rangeSize

    return count
```

## Complexity

Let `N` be the number of input ranges and `K` be the number of merged ranges.

If we solve Part 2 from scratch, the running time is dominated by sorting the
ranges:

```text
Θ(N log N)
```

After the ranges have already been sorted and merged, the final counting step is
just a linear scan over the merged list:

```text
Θ(K)
```

Since `K <= N`, this is bounded by:

```text
Θ(N)
```

The extra space used by the merged range list is:

```text
Θ(N)
```

## Files

- `main.cpp` — reads the input file and prints both answers
- `day05.h` — declarations for `Range` and `Cafeteria`
- `day05.cpp` — input parsing, range merging, binary search, and solution logic

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day05.cpp -o day05
```

## Run

```bash
./day05
```

The program expects the puzzle input in a file named `input.txt`.
