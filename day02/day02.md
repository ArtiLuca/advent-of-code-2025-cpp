# Advent of Code 2025 — Day 2: Gift Shop

# Part 1

In Part 1, we are given a set of comma-separated interval ranges corresponding to the gift shop product IDs.

We are given the task of finding all the invalid IDs in each range and calculating their total sum.

An ID is invalid if the entire ID consists of a sequence of digits repeated exactly twice, meaning that the first half matches the second half, and does not have any leading zeroes. This means that all IDs with an odd-numbered length are automatically valid, as only even-numbered lengths can have perfectly matching first and second halves.

Therefore, I decided to test a middle-split comparison approach, where I treat each ID as a string, verify that its length is even, and compare its first half directly with its second half.

## Idea

Given a string `X` corresponding to the ID being checked, with `n = X.length`, I use a boolean helper to check whether the ID is valid:

* If `n > 0` and `X[0] == '0'`, then the ID is not valid.
* If `n % 2 != 0`, then it is valid, as it cannot be split into two identical halves.
* Otherwise, I calculate the length of one half (`halfLen = n / 2`).
* I check whether the substring from index `0` with length `halfLen` matches the substring from index `halfLen`, also with length `halfLen`.
* If they match, the ID is invalid.

I can then use this to check each ID within a given interval while keeping track of the invalid IDs, so that I can later sum them and obtain the Part 1 solution.

## Pseudocode

```text
checkIfValidID(X)
    n = length(X)

    if n > 0 and X[0] == '0'
        return false

    else if n % 2 != 0
        return true

    else
        halfLen = n / 2

        if X.substring(0, halfLen) == X.substring(halfLen, halfLen)
            return false

    // Otherwise
    return true
```

## Complexity

Let `D` be the length of a single product ID. Converting a `long long` to a string costs `Θ(D)`, while comparing the two halves also costs `Θ(D)`.

Let `R` be the number of IDs checked. The total time complexity is therefore:

`T(R, D) = Θ(R × D)`

The space complexity is `Θ(D)`.

# Part 2

In Part 2, we are told that an ID is invalid if it is made only of some sequence of digits repeated at least twice and does not have any leading zeroes.

For this, I opted for a base-sequence matching approach, where each ID is treated as a string and I find substrings at the start of the string whose lengths perfectly divide the total string length. I can then check whether repeating them matches the original string.

## Idea

Given an ID as a string `X`, with `n = X.length`, I can use an updated boolean helper to check whether the ID is valid or invalid:

* The leading-zero check remains the same.
* The main difference is that I now check substrings at the beginning of the string, with lengths going from `len = 1` to `len = n / 2`.
* For any `len` for which `n` is perfectly divisible by `len`, I reconstruct a test string by repeating that starting substring `n / len` times.
* If the reconstructed string perfectly matches the original string `X`, then the ID is invalid.

Unlike in Part 1, an ID with an odd-numbered length is not automatically valid, because a shorter sequence may be repeated an odd number of times.

I can then use this boolean helper to check each ID in the interval range.

## Pseudocode

```text
checkIfValidID(X)
    n = length(X)

    if n > 0 and X[0] == '0'
        return false

    else
        maxLen = n / 2

        for len = 1 to maxLen
            if n % len == 0
                base = X.substring(0, len)
                initialize rebuilt as an empty string
                repetitions = n / len

                for r = 0 to repetitions - 1
                    rebuilt = rebuilt + base

                if rebuilt == X
                    return false

    // Otherwise
    return true
```

## Complexity

Let `D` be the length of a single product ID. The outer loop runs up to `D / 2` times. Each time we enter the inner loop, we build a string of length `D` by appending chunks, with a cost proportional to `D`. The final comparison also takes `Θ(D)` operations.

Therefore, the worst-case time complexity for checking one ID is:

`O((D / 2) × D) = O(D²)`

Let `R` be the number of IDs checked. The total worst-case time complexity is therefore:

`O(R × D²)`

The space complexity remains `Θ(D)`.
