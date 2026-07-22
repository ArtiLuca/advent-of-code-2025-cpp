# Advent of Code 2025 — Day 6: Trash Compactor

## Part 1

In Part 1, the input represents a wide two-dimensional worksheet containing
several independent arithmetic problems.

Each problem occupies a rectangular block of the worksheet. Its numbers are
written horizontally on separate rows, while the bottom row contains either a
`+` or `*` operator.

Different problems are separated by at least one completely blank column.

The goal is to:

1. identify each problem block;
2. extract its numbers and operator;
3. evaluate the operation;
4. return the sum of all problem results.

## Idea

The main challenge is parsing the worksheet.

Reading the grid one column at a time would separate the digits of horizontal
multi-digit numbers. For example, `328` would incorrectly be treated as three
independent digits.

Instead, I divide the parsing into three phases.

### Phase 1 — Normalize the grid

The input rows may have different lengths.

While reading the file, I record the maximum row width. I then right-pad every
shorter row with spaces so that the worksheet becomes a rectangular grid with
uniform indexing.

### Phase 2 — Find problem boundaries

I scan the worksheet column by column.

A column belongs to a problem block when it contains at least one non-space
character. A completely blank column marks the boundary between two problems.

I keep a `startCol` index:

- when I encounter the first non-empty column, I store its index;
- when I later encounter an empty column, the current problem occupies
  `[startCol, currentCol - 1]`;
- if the worksheet ends during a problem, I process the final block separately.

### Phase 3 — Parse one problem block

Once the horizontal boundaries of a problem are known, I inspect the block
row by row.

For each row:

- if the row contains `+` or `*`, I store the operator;
- otherwise, I scan the row from left to right and construct complete
  multi-digit numbers.

To construct a number, I repeatedly shift the current value by one decimal
position and add the new digit:

```text
currentNumber = currentNumber × 10 + digit
```

## Pseudocode — Finding problem blocks

```text
parseWorksheetPart1(G)
    R = number of rows in G
    C = number of columns in G

    startCol = -1
    operations = empty list

    for col = 0 to C - 1
        columnHasContent = false

        for row = 0 to R - 1
            if G[row, col] != ' '
                columnHasContent = true
                break

        if columnHasContent and startCol == -1
            startCol = col

        else if not columnHasContent and startCol != -1
            parseBlockPart1(G, startCol, col - 1, operations)
            startCol = -1

    if startCol != -1
        parseBlockPart1(G, startCol, C - 1, operations)

    return operations
```

## Pseudocode — Parsing a Part 1 block

```text
parseBlockPart1(G, startCol, endCol, operations)
    currentOperation = new empty Operation
    R = number of rows in G

    for row = 0 to R - 1
        rowSegment = characters G[row, startCol ... endCol]

        hasOperator = false

        for each character c in rowSegment
            if c == '+' or c == '*'
                currentOperation.type = c
                hasOperator = true
                break

        if not hasOperator
            currentNumber = 0
            readingNumber = false

            for each character c in rowSegment
                if c is a digit
                    currentNumber =
                        currentNumber × 10 + numericValue(c)

                    readingNumber = true

                else if readingNumber
                    append currentNumber to currentOperation.numbers
                    currentNumber = 0
                    readingNumber = false

            if readingNumber
                append currentNumber to currentOperation.numbers

    if currentOperation contains numbers
        append currentOperation to operations
```

## Evaluating an operation

Once a block has been parsed, the arithmetic calculation is a simple reduction
over its list of numbers.

For addition, the result starts at `0`. For multiplication, it starts at `1`.

```text
calculateOperation(operation)
    if operation.numbers is empty
        return 0

    if operation.type == '+'
        result = 0
    else
        result = 1

    for each number in operation.numbers
        if operation.type == '+'
            result = result + number
        else
            result = result × number

    return result
```

The final answer is the sum of all operation results:

```text
solvePart1(operations)
    total = 0

    for each operation in operations
        total = total + calculateOperation(operation)

    return total
```

## Complexity

Let:

- `R` be the number of worksheet rows;
- `C` be the width of the padded worksheet;
- `K` be the total number of integers extracted from all problem blocks.

Reading and padding the worksheet requires `Θ(RC)` time.

Finding the problem boundaries also requires `Θ(RC)` time because each column
may be inspected across all rows.

The problem blocks occupy disjoint column ranges, so parsing all blocks together
examines at most `Θ(RC)` cells.

Finally, evaluating the operations costs `Θ(K)`. Since all numbers come from
the worksheet, `K = O(RC)`.

Therefore, the complete time complexity is:

```text
Θ(RC)
```

The padded worksheet occupies:

```text
Θ(RC)
```

additional space. The parsed operations are also bounded by the worksheet size,
so the total auxiliary space complexity remains:

```text
Θ(RC)
```

---

## Part 2

In Part 2, the same worksheet is interpreted using cephalopod mathematics.

Instead of reading each number horizontally, every column inside a problem block
forms one number.

The digits are read vertically from top to bottom:

- the top digit is the most significant digit;
- the bottom digit is the least significant digit;
- the operator remains on the final worksheet row.

The columns themselves are read from right to left.

## Idea

The first two phases remain unchanged:

1. normalize the worksheet into a rectangular grid;
2. identify problem blocks using completely blank columns.

Only the block parser changes.

For each block:

1. inspect the bottom row to find the operator;
2. scan the block columns from right to left;
3. scan each column from top to bottom, excluding the operator row;
4. combine the digits in that column into one integer;
5. store the resulting number in the current operation.

Addition and multiplication are commutative, so reversing the order of the
operands would not change the final result. However, I still scan right to left
to follow the problem specification directly.

## Pseudocode — Parsing a Part 2 block

```text
parseBlockPart2(G, startCol, endCol, operations)
    currentOperation = new empty Operation
    R = number of rows in G

    for col = startCol to endCol
        c = G[R - 1, col]

        if c == '+' or c == '*'
            currentOperation.type = c
            break

    for col = endCol downto startCol
        currentNumber = 0
        readingNumber = false

        for row = 0 to R - 2
            c = G[row, col]

            if c is a digit
                currentNumber =
                    currentNumber × 10 + numericValue(c)

                readingNumber = true

        if readingNumber
            append currentNumber to currentOperation.numbers

    if currentOperation contains numbers
        append currentOperation to operations
```

The same arithmetic reduction used in Part 1 can then evaluate every parsed
operation.

## Complexity

Part 2 performs the same grid normalization and boundary scan as Part 1.

Every cell inside every problem block is inspected a constant number of times,
so the total time complexity remains:

```text
Θ(RC)
```

The padded worksheet and parsed operations require:

```text
Θ(RC)
```

additional space.

## Files

- `main.cpp` — runs both parts and prints their answers
- `day06.h` — declares `Operation` and `CephalopodMath`
- `day06.cpp` — handles grid normalization, block parsing, and evaluation

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day06.cpp -o day06
```

## Run

```bash
./day06
```

The program expects the puzzle input in a file named `input.txt`.
