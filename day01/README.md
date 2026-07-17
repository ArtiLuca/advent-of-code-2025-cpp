# Advent of Code 2025 — Day 1: Secret Entrance

C++ solution for Day 1 of Advent of Code 2025.

The program reads a sequence of dial rotations. The dial contains the values
`0` through `99`, begins at `50`, and wraps around in both directions.

## Part 1

For each rotation, the new dial position is calculated using modulo 100:

- right rotation: `(current + distance) mod 100`
- left rotation: `(current - distance) mod 100`

Negative results from a left rotation are normalized back into the interval
`[0, 99]`.

The answer is the number of rotations that finish with the dial positioned at
exactly `0`.

## Part 2

Part 2 counts every time the dial passes through or lands on `0`, including
rotations that complete more than one full turn.

The number of zero crossings is calculated directly rather than simulating
each individual click of the dial.

## Structure

- `main.cpp` — loads the input and prints both answers
- `day1.h` — declarations for `Rotation` and `Safe`
- `day1.cpp` — parsing and solution implementation

## Complexity

Let `n` be the number of rotations.

- Time: `O(n)`
- Space: `O(n)` to store the rotations

Each rotation is processed in constant time.

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day1.cpp -o day1
