# Advent of Code 2025 — Day 3: Lobby

## Part 1

In Part 1, we are dealing with batteries and their joltage ratings, which can be any value in `[1..9]`.

The batteries are organized into banks, and they cannot be rearranged.

We must turn on exactly two batteries in each bank to form the largest possible 2-digit number ("joltage") while preserving their original order.

The largest combination always starts with the highest digit in the string, with the only exception being when the first occurrence of the largest digit is the very last one, in which case it cannot physically be used as the first digit.

Therefore, I decided to test a two-stage scan approach where I locate the global maximum (`max1`) first and then find the second largest digit (`max2`), depending on whether the first one found was the last digit or not.

As the highest possible digit value is `9`, I can also optimize this approach by adding early-exit conditions whenever I encounter a `9` during a scan.

## Idea

Given a string `X` corresponding to the battery bank being checked, with `n = X.length`, I first look for the highest digit `max1` and the index `idx1` of its first occurrence.

Then, I split the algorithm into two cases:

* **Case 1:** If `idx1` is not the last index, then the largest combination must start with this maximum digit `max1`. I scan the remaining digits to its right, from `idx1 + 1` to `n - 1`, to find the largest available second digit `max2` to pair with it.
* **Case 2:** If `idx1` is the very last index, it cannot be used as the first digit. I scan the digits to its left, from `0` to `idx1 - 1`, to find the largest possible first digit `max2`.

## Pseudocode

```text
findJoltage(X)
    n = length(X)

    if n < 2
        return -1

    // Find the first maximum
    max1 = '/'
    idx1 = -1

    for i = 0 to n - 1
        if X[i] > max1
            max1 = X[i]
            idx1 = i

        if max1 == '9'
            break

    // Case 1
    if idx1 < n - 1
        max2 = '/'

        for j = idx1 + 1 to n - 1
            if X[j] > max2
                max2 = X[j]

            if max2 == '9'
                break

        return (max1 - '0') * 10 + (max2 - '0')

    // Case 2
    else
        max2 = '/'

        for j = 0 to idx1 - 1
            if X[j] > max2
                max2 = X[j]

            if max2 == '9'
                break

        return (max2 - '0') * 10 + (max1 - '0')
```

## Complexity

Let `B` be the length of a single bank of batteries and let `L` be the total number of battery banks we check.

The time complexity for a single bank is `Θ(B)`, as we scan the string at most twice.

Therefore, if all banks have length `B`, the overall time complexity is:

`T(B, L) = Θ(B × L)`

More generally, if the banks have different lengths, the total time is proportional to the sum of their lengths.

The auxiliary space complexity is `Θ(1)`.

# Part 2

In Part 2, the problem made me drastically rethink the approach, as we now have to turn on 12 batteries instead of 2.

The approach used for Part 1 would not work well, so I decided to use a greedy approach.

The problem can be defined in more general terms as follows:

Given a bank of batteries `B[0..n-1]` and a fixed value `k ≤ n`—in this case, `k = 12`—we must find a subsequence of length exactly `k` that maximizes the joltage rating from left to right while preserving the original order.

## Idea

Given a string `X` corresponding to the battery bank being checked, with `n = X.length`, if we must select `k` digits from the `n` available, this means that we can discard exactly `n - k` digits.

To maximize the result, the largest digits should be placed as far to the left as possible.

We can scan `X` from left to right and consider the current digit `X[i]`.

If the current digit `X[i]` is strictly greater than the last element we chose to keep and we still have discards available, then it is locally optimal to discard the previous element and replace it with the current digit `X[i]`.

We can implement this greedy logic by simulating a stack structure `S` and using a pointer `top` to the last element inserted into `S`.

## Pseudocode

In our case, `n = X.length` and `k = 12`, but the algorithm can be defined for any given `k ≤ n`:

```text
findJoltagePart2(X, n, k)

    // Allocate the stack structure
    allocate S[0..n-1]

    // Pointer to the last element inserted into S
    top = -1

    // Number of discards still available
    discard = n - k

    // Scan X from left to right
    for i = 0 to n - 1
        current = X[i] - '0'

        // While S contains digits, the last digit inserted
        // is smaller than the current digit, and we still
        // have discards available, remove it from S
        while top >= 0 and S[top] < current and discard > 0
            top = top - 1
            discard = discard - 1

        // Insert the current element at the top
        top = top + 1
        S[top] = current

    // If some discards remain, remove the excess
    // digits from the end
    while discard > 0
        top = top - 1
        discard = discard - 1

    // Build the final number using the first k elements
    result = 0

    for j = 0 to k - 1
        result = (result * 10) + S[j]

    return result
```

## Complexity

Let `N` be the length of a bank of batteries and let `M` be the number of battery banks we check.

At first glance, the nested loops may make the complexity for a single bank appear to be `O(N²)`. However, using amortized analysis, we can demonstrate that the cost for a single bank is linear.

Each digit enters the stack exactly once: the outer `for` loop scans the string from left to right and inserts every current digit into the stack. Therefore, with a string of length `N`, we perform `N` insertions.

Each digit leaves the stack at most once: the inner `while` loop can only remove digits that are currently in the stack. Therefore, there can be at most `N` removals from the stack.

If we sum the insertions and removals performed on the stack `S`, the upper bound on the number of these operations is `2N`, meaning that the time complexity for one bank is `Θ(N)`.

Therefore, if all `M` battery banks have length `N`, the overall time complexity is:

`T(N, M) = Θ(M × N)`

More generally, if the banks have different lengths, the total time is proportional to the sum of their lengths.

The auxiliary space complexity is `Θ(N)` for the stack structure used for the largest battery bank being evaluated, as we can reuse the same memory space across multiple banks without storing all `M` results simultaneously.

# Formal Definitions

This section is solely to train myself in writing formal definitions.

## Part 1

Formally, the problem for Part 1 can be defined as follows.

Given a string $X$ of length $n \ge 2$, we operate piecewise based on the position of the global maximum using the two-scan approach.

For clarity, let $X[i]$ denote the numerical value of the digit at index $i$.

First, we find the index $i^*$ of the first occurrence of the maximum digit in $X$:

$$
i^* =
\min
\left{
i \in {0,\ldots,n-1}
;\middle|;
X[i] = \max_{0 \le m < n} X[m]
\right}.
$$

Depending on the position of $i^*$, there are two mutually exclusive cases for finding a secondary index $j^*$.

### Case 1: $i^* < n-1$

The maximum is not the last element, so it can serve as the most significant digit. We scan the remaining suffix and choose the first occurrence of its largest available digit:

$$
j^* =
\min
\left{
j \in {i^*+1,\ldots,n-1}
;\middle|;
X[j] = \max_{i^* < m < n} X[m]
\right}.
$$

The resulting joltage is:

$$
10X[i^*] + X[j^*].
$$

### Case 2: $i^* = n-1$

The maximum digit is the last element and is therefore constrained to be the least significant digit. We scan the preceding prefix and choose the first occurrence of its largest available digit:

$$
j^* =
\min
\left{
j \in {0,\ldots,i^*-1}
;\middle|;
X[j] = \max_{0 \le m < i^*} X[m]
\right}.
$$

The resulting joltage is:

$$
10X[j^*] + X[i^*].
$$

The complete result can therefore be written as:

$$
J_2(X) =
\begin{cases}
10X[i^*] + X[j^*], & \text{if } i^* < n-1, [4pt]
10X[j^*] + X[i^*], & \text{if } i^* = n-1.
\end{cases}
$$

## Part 2

Formally, the problem for Part 2 can be defined as follows.

Given a string $X$ of length $n$, composed of digits from the alphabet

$$
\Sigma = {1,2,\ldots,9},
$$

and an integer $k \in \mathbb{Z}$ such that

$$
1 \le k \le n,
$$

we must find a subsequence $X'$ of length exactly $k$.

A valid subsequence is defined by a sequence of indices satisfying:

$$
0 \le i_1 < i_2 < \cdots < i_k < n.
$$

The numerical value associated with the chosen subsequence is:

$$
V(i_1,\ldots,i_k)
=================

\sum_{j=1}^{k}
X[i_j] \cdot 10^{k-j}.
$$

The goal is to find indices $i_1^*,\ldots,i_k^*$ such that:

$$
(i_1^*,\ldots,i_k^*)
\in
\operatorname*{arg,max}_{
0 \le i_1 < i_2 < \cdots < i_k < n
}
V(i_1,\ldots,i_k).
$$

Equivalently:

$$
V(i_1^*,\ldots,i_k^*)
=====================

\max_{
0 \le i_1 < i_2 < \cdots < i_k < n
}
\sum_{j=1}^{k}
X[i_j] \cdot 10^{k-j}.
$$

Because all candidate subsequences have the same fixed length $k$, maximizing their numerical value is equivalent to finding the lexicographically largest subsequence of length $k$.
