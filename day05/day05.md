# Advent of Code 2025 — Day 5: Cafeteria

# Part 1

We are working with prducts or ingredients.

These IDs are first given as a set of ranges of product IDs for ingredients that are considered "fresh."

These ranges are formatted as `[start-end]`. They are inclusive and can overlap.

We are then given a list of standalone product IDs to check.

Specifically, we want to calculate the total number of standalone product IDs that fall within at least one of the fresh ranges.

Product IDs that do not fall within any range are considered "spoiled."

## Idea

If we have `P` product IDs and `N` ranges, checking every product ID against every single range one by one yields an inefficient running time of:

```text
O(P × N)
```

Therefore, after parsing the input, the ranges must be ordered so that we can search through them efficiently.

The sorting can be performed based on the starting values of the ranges. We then pass through them linearly to combine overlapping ranges.

### Note

The sorting can be performed using `std::sort` or through a careful implementation of MergeSort.

I learned that `std::sort` is commonly implemented using an introspective sorting strategy, which combines QuickSort with a HeapSort fallback and may use InsertionSort for small partitions.

I did not want to reinvent the wheel, so I ended up using `std::sort`.

However, to practise my Data Structures & Algorithms logic, I also wrote down a possible MergeSort implementation for this problem.

Having sorted and merged the ranges, I can then use a divide-and-conquer approach through an iterative binary search.

For each ID, we check the middle range. Since the ranges are non-overlapping, the target ID can only belong to one of three cases:

1. It is inside the middle range, so we have found a match.
2. It is strictly smaller than the middle range's starting value, so we can safely discard the right half.
3. It is strictly greater than the middle range's ending value, so we can safely discard the left half.

We use an iterative binary search instead of a recursive one to avoid allocating extra call-stack memory.

## Implementation Architecture

In the final C++ class implementation, Phase 1—sorting—and Phase 2—collapsing overlaps—are combined into a single private helper method called `prepareRanges()`.

This ensures data reuse and cleaner encapsulation when evaluating Part 1 and Part 2.

## Pseudocode

I decided to use `std::sort` for the actual sorting of the ranges. However, it works towards the same result as a customized version of MergeSort such as the one shown below.

I assume that each range in `R[0..N-1]` has access to the fields `start` and `end`.

I split the solution into three phases.

### Phase 1: Sorting

```text
// Main recursive function
// R[0..N-1] contains all ranges
mergeSort(R, p, r)
    if p < r
        q = p + (r - p) / 2
        mergeSort(R, p, q)       // Sort left half
        mergeSort(R, q + 1, r)   // Sort right half
        merge(R, p, q, r)        // Merge the two halves


// Merge procedure
merge(R, p, q, r)
    allocate copyR[0..r-p]

    i = p
    j = q + 1
    k = 0

    // Compare and merge based on the starting value
    // of each range
    while i <= q and j <= r
        if R[i].start <= R[j].start
            copyR[k] = R[i]
            k = k + 1
            i = i + 1
        else
            copyR[k] = R[j]
            k = k + 1
            j = j + 1

    // Copy any remaining elements
    while i <= q
        copyR[k] = R[i]
        k = k + 1
        i = i + 1

    while j <= r
        copyR[k] = R[j]
        k = k + 1
        j = j + 1

    // Copy the result back into the original array
    k = 0

    for i = p to r
        R[i] = copyR[k]
        k = k + 1
```

### Phase 2: Collapsing Overlaps

Once the initial ranges are sorted, we collapse overlapping ranges into a new list `M` using a linear pass.

```text
mergeOverlaps(R)
    allocate M as an empty list of ranges

    if R is empty
        return M

    M.push_back(R[0])

    for i = 1 to R.size - 1
        current = R[i]
        last = M.last

        // If there is no overlap, add the current
        // range as a new distinct range
        if current.start > last.end
            M.push_back(current)

        // Otherwise, collapse the overlap
        else
            M.last.end = max(M.last.end, current.end)

    return M
```

This combines overlapping ranges. Adjacent but non-overlapping ranges do not need to be combined for either part of this problem.

### Phase 3: Binary Search

Once the ranges are sorted and non-overlapping, the binary search can be implemented as follows:

```text
binarySearch(M, id)
    p = 0
    r = M.size - 1

    while p <= r
        q = p + (r - p) / 2

        // Case 1: found a match
        if id >= M[q].start and id <= M[q].end
            return true

        // Case 2: discard the right half
        else if id < M[q].start
            r = q - 1

        // Case 3: discard the left half
        else
            p = q + 1

    return false
```

Once Phases 1–3 have been executed, we can count the fresh product IDs.

```text
// M is the list generated during Phase 2
// P[0..n-1] is the list of standalone product IDs
countFresh(M, P)
    count = 0

    for i = 0 to P.size - 1
        if binarySearch(M, P[i]) == true
            count = count + 1

    return count
```

## Complexity

Let:

* `N` be the total number of initial input ranges;
* `P` be the total number of standalone product IDs;
* `K` be the number of merged ranges, where `K ≤ N`.

The cost of the Part 1 solution is divided into three blocks:

1. Sorting the ranges takes `O(N log N)` time when using `std::sort`. A MergeSort implementation would take `Θ(N log N)`.
2. Collapsing the ranges performs a linear scan and takes `Θ(N)`.
3. The counting step loops through all `P` product IDs. During each iteration, the iterative binary search cuts the search space of `M` in half and costs `O(log K)`.

The total worst-case time complexity is therefore:

```text
O(N log N) + Θ(N) + O(P log K)
```

Since `K ≤ N`, this can be bounded by:

```text
O(N log N + P log N)
```

or, equivalently:

```text
O((N + P) log N)
```

The merged-range list can contain up to `N` ranges, so it requires:

```text
Θ(N)
```

additional space.

If the optional MergeSort implementation is used, its temporary arrays also require `Θ(N)` auxiliary space.

# Part 2

In Part 2, we are told that we no longer have to check the standalone product IDs.

Instead, we use the ranges to count the total number of fresh products, meaning the total number of unique IDs covered by the inclusive ranges.

Thanks to the solution from Part 1 and the `mergeOverlaps` phase, we have already collapsed all overlapping ranges into the list `M`, which is represented by the vector `mergedRanges` in the actual implementation.

## Idea

Since the list `M`, or vector `mergedRanges`, produced during Phase 2 of Part 1 contains disjoint ranges, we can find the total count by simply computing the span of each collapsed range.

The number of IDs inside a single inclusive range `[start, end]` is exactly:

```text
end - start + 1
```

We can therefore iterate through `M` and sum the size of each range.

## Pseudocode

Only Phases 1 and 2 are required before performing this calculation.

```text
// M is the list produced during Phase 2
countFreshRanges(M)
    count = 0

    for i = 0 to M.size - 1
        currentRange = M[i]

        // Compute the size of the current inclusive range
        rangeSize = currentRange.end - currentRange.start + 1
        count = count + rangeSize

    return count
```

## Complexity

Let:

* `N` be the total number of initial input ranges;
* `K` be the number of merged ranges, where `K ≤ N`.

The overall worst-case time complexity for Part 2 when starting from the original unsorted ranges is dominated by the initial sorting phase:

```text
O(N log N)
```

If we isolate the calculation performed after the sorting and collapsing phases have already been completed, computing the sizes of the merged ranges requires a single linear pass over `M`:

```text
Θ(K)
```

Since `K ≤ N`, this is `O(N)` and is `Θ(N)` in the worst case, when none of the original ranges overlap.

The merged-range list requires up to:

```text
Θ(N)
```

additional space.
