# Advent of Code 2025 — Day 1: Secret Entrance

The input contains a sequence of rotations for a circular dial.

The dial has positions from `0` to `99`, starts at position `50`, and wraps
around in both directions. Each instruction contains:

- `R` for a clockwise rotation;
- `L` for a counter-clockwise rotation;
- the number of positions by which the dial must move.

## Part 1

In Part 1, the goal is to count how many rotations finish with the dial exactly
at position `0`.

## Idea

Because the dial is circular, its position can be updated using modulo `100`.

For a right rotation:

```text
newPosition = (currentPosition + distance) mod 100
```

For a left rotation, the subtraction may produce a negative value. I therefore
normalize the result back into the interval `[0, 99]`:

```text
newPosition =
    ((currentPosition - distance) mod 100 + 100) mod 100
```

After every rotation, I check whether the resulting position is `0`.

## Pseudocode

```text
solvePart1(rotations)
    position = 50
    count = 0

    for each rotation in rotations
        if rotation.direction == 'R'
            position =
                (position + rotation.distance) mod 100

        else
            position =
                ((position - rotation.distance) mod 100 + 100) mod 100

        if position == 0
            count = count + 1

    return count
```

## Complexity

Let `N` be the number of rotations.

Each rotation is processed using a constant number of arithmetic operations, so
the time complexity is:

```text
Θ(N)
```

The current implementation stores all rotations before solving the puzzle, so
the total additional space is:

```text
Θ(N)
```

The solving step itself uses only constant additional space.

---

## Part 2

In Part 2, the goal changes slightly.

Instead of counting only the rotations that finish at `0`, I must count every
time the dial passes through or lands on `0` while rotating.

A single instruction may travel more than one complete turn, so it may cross
`0` several times.

## Idea

A direct simulation could move the dial one position at a time and check every
click. However, a rotation with distance `D` would then require `Θ(D)` time.

Instead, I calculate the number of zero crossings directly using arithmetic.

For a right rotation, the dial crosses `0` whenever the movement passes another
multiple of `100`. Starting from position `position`, the number of crossings is:

```text
(position + distance) / 100
```

using integer division.

For a left rotation, the distance to the next `0` depends on the current
position. The number of crossings can be calculated as:

```text
(distance + (100 - position) mod 100) / 100
```

After counting the crossings, I update the final dial position using the same
modulo logic as in Part 1.

This avoids simulating every individual movement of the dial.

## Pseudocode

```text
solvePart2(rotations)
    position = 50
    count = 0

    for each rotation in rotations
        distance = rotation.distance

        if rotation.direction == 'R'
            count = count + (position + distance) / 100
            position = (position + distance) mod 100

        else
            distanceToZero = (100 - position) mod 100

            count =
                count + (distance + distanceToZero) / 100

            position =
                ((position - distance) mod 100 + 100) mod 100

    return count
```

## Why the direct calculation works

During a rotation, the dial repeats its positions every `100` movements.

Therefore, every complete group of `100` clicks contributes exactly one passage
through `0`. The remaining partial movement contributes another crossing only
when it reaches the next occurrence of `0`.

The arithmetic formulas calculate exactly how many such boundaries are crossed,
without examining every intermediate dial position.

## Complexity

Let `N` be the number of rotations.

The number of zero crossings for each rotation is calculated in constant time,
regardless of the rotation distance.

Therefore, the time complexity is:

```text
Θ(N)
```

The current implementation stores the rotations, so the total additional space
is:

```text
Θ(N)
```

The arithmetic solving process itself requires only constant additional space.

## Files

- `main.cpp` — loads the input and prints both answers
- `day1.h` — declares `Rotation` and `Safe`
- `day1.cpp` — handles input parsing and implements both solutions

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day1.cpp -o day1
```

## Run

```bash
./day1
```

The program expects the puzzle input in a file named `input.txt`.
