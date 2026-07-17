# Advent of Code 2025 — Day 2: Gift Shop

## Part 1

In Part 1, we are given a set of comma-separated interval ranges corresponding
to gift shop product IDs.

The task is to find all invalid IDs in the given ranges and return their total
sum.

An ID is invalid if its decimal representation consists of a sequence of digits
repeated exactly twice. In other words, the first half of the ID must match the
second half.

For example:

- `55` is invalid because it is `5` repeated twice.
- `6464` is invalid because it is `64` repeated twice.
- `123123` is invalid because it is `123` repeated twice.

IDs with an odd number of digits are automatically valid for Part 1, because
they cannot be split into two equal halves.

### Idea

For each ID, I convert the number to a string.

If the length of the string is odd, I return that the ID is valid. Otherwise, I
split the string into two equal halves and compare them directly.

If the two halves match, the ID is invalid.

### Pseudocode

```text
isValidID(id)
    text = convert id to string
    n = length(text)

    if n is odd
        return true

    halfLength = n / 2

    if text[0 .. halfLength-1] == text[halfLength .. n-1]
        return false

    return true
```

## Part 2

In Part 2, the definition of invalid ID is more general.

An ID is invalid if it is made from a shorter sequence of digits repeated at
least twice.

For example:

- `12341234` is invalid because it is `1234` repeated twice.
- `123123123` is invalid because it is `123` repeated three times.
- `1212121212` is invalid because it is `12` repeated five times.
- `1111111` is invalid because it is `1` repeated seven times.

### Idea

For each ID, I again treat the number as a string.

This time, instead of only checking whether the first half matches the second
half, I test every possible prefix length from `1` to `n / 2`.

A prefix can only be a valid repeated pattern if its length divides the total
length of the ID. For each valid prefix length, I rebuild the string by
repeating the prefix and then compare the rebuilt string with the original ID.

If the rebuilt string matches the original ID, then the ID is invalid.

### Pseudocode

```text
isValidIDPart2(id)
    text = convert id to string
    n = length(text)

    for length = 1 to n / 2
        if n mod length == 0
            pattern = text[0 .. length-1]
            rebuilt = empty string
            repetitions = n / length

            repeat repetitions times
                append pattern to rebuilt

            if rebuilt == text
                return false

    return true
```

## Complexity

Let:

- `R` be the total number of IDs checked across all ranges;
- `D` be the maximum number of digits in an ID.

### Part 1

For each ID, converting the number to a string costs `Θ(D)`. Comparing the two
halves also costs at most `Θ(D)`.

Therefore:

- Time complexity: `Θ(R × D)`
- Space complexity: `Θ(D)`

### Part 2

For each ID, the algorithm checks possible pattern lengths up to `D / 2`.

For each candidate length that divides `D`, it may rebuild a string of length
`D` and compare it with the original string.

Therefore:

- Worst-case time complexity: `O(R × D²)`
- Space complexity: `Θ(D)`

In practice, the input numbers have relatively few digits, so this direct string
based approach is fast enough for the puzzle.

## Files

- `main.cpp` — reads the input file and prints both answers
- `day02.h` — declarations for the main types and functions
- `day02.cpp` — input parsing and solution logic

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day02.cpp -o day02
```

## Run

```bash
./day02
```

The program expects the puzzle input in a file named `input.txt`.
