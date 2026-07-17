# Advent of Code 2025 — Day 2: Gift Shop

C++ solution for Day 2 of Advent of Code 2025.

The input contains a collection of comma-separated, inclusive ranges of product
IDs. For every range, the program identifies invalid IDs and calculates their
total sum.

## Part 1

An ID is invalid when its decimal representation consists of the same sequence
of digits repeated exactly twice.

Examples of invalid IDs include:

- `55`, which is `5` repeated twice;
- `6464`, which is `64` repeated twice;
- `123123`, which is `123` repeated twice.

An ID containing an odd number of digits cannot be divided into two identical
halves and is therefore valid for Part 1.

### Approach

Each ID is converted to a string.

1. If the string length is odd, the ID is valid.
2. Otherwise, divide the string into two equally sized halves.
3. If the first half matches the second half, the ID is invalid.
4. Add every invalid ID to the total sum.

### Pseudocode

```text
isValidPart1(id)
    text = convert id to string
    length = text.length

    if length is odd
        return true

    halfLength = length / 2

    if text[0 .. halfLength-1] equals text[halfLength .. length-1]
        return false

    return true
```

## Part 2

In Part 2, an ID is invalid when its decimal representation consists of a
shorter sequence of digits repeated at least twice.

Examples of invalid IDs include:

- `12341234`, which is `1234` repeated twice;
- `123123123`, which is `123` repeated three times;
- `1212121212`, which is `12` repeated five times;
- `1111111`, which is `1` repeated seven times.

### Approach

For an ID with `D` digits, a repeated base pattern can only have length between
`1` and `D / 2`.

For each candidate pattern length:

1. Check whether the candidate length divides `D` exactly.
2. Use the corresponding prefix as the candidate base pattern.
3. Repeat that pattern until a string of length `D` is rebuilt.
4. Compare the rebuilt string with the original ID.
5. If they match, the ID is invalid.

Only pattern lengths that divide the total number of digits need to be tested.

### Pseudocode

```text
isValidPart2(id)
    text = convert id to string
    length = text.length

    for patternLength = 1 to length / 2
        if length mod patternLength == 0
            pattern = prefix of text with patternLength characters
            rebuilt = empty string
            repetitions = length / patternLength

            repeat repetitions times
                append pattern to rebuilt

            if rebuilt equals text
                return false

    return true
```

## Complexity

Let:

- `R` be the total number of IDs examined across all ranges;
- `D` be the maximum number of digits in an ID.

### Part 1

Each ID is converted to a string, and at most `D` characters are compared.

- Time: `Θ(R × D)`
- Temporary space: `Θ(D)`

### Part 2

For each ID, at most `D / 2` candidate pattern lengths are considered. For each
candidate length that divides `D`, the algorithm may rebuild and compare a
string of length `D`.

- Worst-case time: `O(R × D²)`
- Temporary space: `Θ(D)`

The actual cost is usually lower because many candidate pattern lengths are
skipped when they do not divide the number of digits.

## Project structure

- `main.cpp` — loads the input and prints both answers
- `day02.h` — declarations for `Interval` and `GiftShop`
- `day02.cpp` — input parsing and solution implementation

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day02.cpp -o day02
```

## Run

```bash
./day02
```

The program expects the puzzle input in a file named `input.txt`.