# Advent of Code 2025 — Day 3: Lobby

## Part 1

In Part 1, we are dealing with banks of batteries. Each battery has a joltage
rating represented by a digit from `1` to `9`.

For each bank, we must turn on exactly two batteries to form the largest
possible 2-digit joltage value. The batteries cannot be rearranged, so the two
digits must be chosen while preserving their original order.

For example, if the bank is represented as a string, we are looking for the
largest possible 2-digit subsequence.

## Idea

For a battery bank `X` of length `n`, I use a two-stage scan.

First, I look for the first occurrence of the largest digit in the string. If
this digit is not the last digit, then it can be used as the first digit of the
2-digit joltage. I then scan the remaining suffix to find the largest possible
second digit.

The only special case is when the largest digit appears for the first time at
the very last position. In that case, it cannot be used as the first digit, so I
scan the prefix before it and choose the largest possible first digit instead.

Since the highest possible digit is `9`, the scans can stop early whenever a
`9` is found.

## Pseudocode

```text
findJoltage(X)
    n = length(X)

    if n < 2
        return error

    max1 = smallest possible digit
    index1 = -1

    for i = 0 to n-1
        if X[i] > max1
            max1 = X[i]
            index1 = i

        if max1 == '9'
            break

    if index1 < n-1
        max2 = smallest possible digit

        for j = index1+1 to n-1
            if X[j] > max2
                max2 = X[j]

            if max2 == '9'
                break

        return number formed by max1 and max2

    else
        max2 = smallest possible digit

        for j = 0 to index1-1
            if X[j] > max2
                max2 = X[j]

            if max2 == '9'
                break

        return number formed by max2 and max1
```

## Part 2

In Part 2, the problem changes from choosing exactly 2 batteries to choosing
exactly 12 batteries.

This made me rethink the Part 1 approach. Instead of writing many special cases,
I treated the problem more generally:

Given a bank of batteries `X[0..n-1]` and a fixed value `k`, find the
lexicographically largest subsequence of length exactly `k`, while preserving
the original order of the digits.

In this puzzle, `k = 12`.

## Idea

If we have `n` digits and must keep exactly `k` of them, then we are allowed to
discard exactly `n - k` digits.

To maximize the final joltage value, we want larger digits to appear as far left
as possible.

I scan the bank from left to right and maintain a stack-like structure `S`
containing the digits chosen so far.

When the current digit is larger than the last digit kept in `S`, and I still
have discards available, it is better to remove the smaller previous digit and
replace it with the current one. This pushes larger digits toward the left side
of the final number.

After the scan, if some discards are still unused, they must correspond to extra
digits at the end, so they can be removed from the back.

## Pseudocode

```text
findJoltagePart2(X, k)
    n = length(X)
    discard = n - k

    S = empty stack-like structure

    for i = 0 to n-1
        current = X[i]

        while S is not empty
              and last element of S < current
              and discard > 0

            remove last element from S
            discard = discard - 1

        append current to S

    while discard > 0
        remove last element from S
        discard = discard - 1

    result = 0

    for j = 0 to k-1
        result = result * 10 + S[j]

    return result
```

## Why the greedy idea works

The final number has a fixed length, so maximizing it is equivalent to making it
lexicographically as large as possible.

When a larger digit appears later, and there are still enough remaining digits
to complete the final number, keeping a smaller digit before it would only make
the result worse. Therefore, it is safe to discard the smaller previous digit.

The stack keeps track of the best prefix found so far, while the `discard`
counter ensures that we never remove too many digits.

## Complexity

Let:

- `N` be the length of a single battery bank;
- `M` be the number of banks.

### Part 1

For each bank, the string is scanned at most twice.

- Time complexity per bank: `Θ(N)`
- Total time complexity: `Θ(M × N)`
- Auxiliary space complexity: `Θ(1)`

### Part 2

At first glance, the nested `while` loop may look quadratic. However, each digit
is inserted into the stack once and removed from the stack at most once.

So, for a single bank, the total number of stack operations is linear.

- Time complexity per bank: `Θ(N)`
- Total time complexity: `Θ(M × N)`
- Auxiliary space complexity: `Θ(N)`

In this puzzle, Part 2 uses `k = 12`, but the same greedy approach works for any
fixed `k <= N`.

## Files

- `main.cpp` — reads the input file and prints both answers
- `day03.h` — declarations for `Bank` and `Escalator`
- `day03.cpp` — input parsing and solution logic

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day03.cpp -o day03
```

## Run

```bash
./day03
```

The program expects the puzzle input in a file named `input.txt`.
