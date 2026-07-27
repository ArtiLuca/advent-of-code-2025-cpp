# Original Notes — Day 6: Trash Compactor

> These are my original notes for this day, kept to show my first reasoning before cleaning the explanation into the final `README.md`.

## Part 1

In Part 1, we are dealing with cephalopods and their math problems, which is a wide 2D worksheet.

The worksheet contains a series of independent arithmetic problem blocks separated by completely blank columns.

Each block contains a vertical arrangement of whole numbers, which may have multiple digits, followed by a single mathematical operator (`+` or `*`) on the bottom row.

We need to parse each individual sub-grid block, extract the multi-digit integers horizontally from each row, evaluate the calculation using the bottom-most mathematical operator, and return the sum of all the computed results.

The math problems are arranged next to one another in a very long horizontal layout. Each problem's numbers are stacked vertically within a block of columns, with the bottom row containing the mathematical operator assigned to that block.

We are told that the problems are separated by a completely blank column and that the internal left or right alignment of the numbers within each problem can be ignored.

## Idea

The main challenge in Part 1 is parsing the input.

If we try to process the characters sequentially down each column in complete isolation, multi-digit values spanning multiple horizontal cells become fragmented into separate digits.

For example, the number `328` occupies 3 columns. Processing each column in isolation would break these digits apart.

Additionally, the problems can have different widths.

To solve this, I decided to process the grid horizontally in bounded problem blocks by separating the work into three distinct phases.

### Phase 1

We allocate a 2D grid containing all the lines read from the input. While reading them, we keep track of the maximum line width.

We do this so that we can right-pad all shorter lines with trailing spaces (`' '`). This gives us a uniform rectangular grid with safe and consistent index boundaries.

### Phase 2

We loop horizontally across the column indices.

By checking the vertical content of each column from top to bottom, we find the exact bounding indices `[startCol, endCol]` of a single problem block.

A problem begins when a column contains at least one non-space character and ends when a completely empty spacing column is found.

These completely blank columns act as boundaries separating the different problems.

### Phase 3

Once we have found a block's column bounds `[startCol, endCol]`, we isolate that sub-grid slice and scan it row by row.

For each row within the block boundaries, we extract the horizontal string segment from `startCol` to `endCol`.

This allows us to read complete multi-digit numbers from left to right without separating their digits.

## Parsing the Input

Parsing the padded grid requires managing both the horizontal block boundaries and the extraction of the numbers from each row.

For the first part, we use a tracking index `startCol`, initialized to `-1`, and scan the columns from left to right.

For each column `j`, we scan vertically through its cells. If the column contains any non-space character, it contains part of a problem.

We then have two cases:

1. If the column has content and `startCol == -1`, we have found the starting boundary of a problem block.
2. If the column is completely empty and `startCol != -1`, we have reached the boundary after the current problem.

In the second case, we pass the range `[startCol, j - 1]` to a dedicated sub-grid procedure and reset `startCol` to `-1`.

The dedicated procedure scans the block vertically, one row at a time. For each row, it extracts the horizontal character segment between `startCol` and `endCol`.

Mathematical operators are detected by checking whether the row segment contains a `+` or `*`.

If it does, we save it as the problem's operator and set a boolean flag called `hasOperator`, marking the row as an operator row.

If it is a number row, we scan the row segment horizontally from left to right.

We keep a `currentNumber` tracker initialized to `0` and a boolean state flag called `readingNumber`.

When we encounter a digit, we subtract the character value `'0'` to obtain its numerical value, shift the existing number left by one decimal position using `currentNumber × 10`, and add the new digit.

When a space is found while `readingNumber` is `true`, the complete multi-digit number has been parsed, so we store it in the list of numbers belonging to the current operation.

## Pseudocode — Parsing

The primary coordinate-bounding procedure evaluates each column and passes the boundaries of every problem block to the row-slice processor.

```text
// G is the padded 2D grid
readAndParseGrid(G)
    R = G.rows
    C = G.columns
    startCol = -1
    operations = empty list

    // Scan horizontally across the column indices
    for j = 0 to C - 1
        columnHasContent = false

        for i = 0 to R - 1
            if G[i, j] != ' '
                columnHasContent = true
                break

        if columnHasContent == true and startCol == -1
            startCol = j

        else if columnHasContent == false and startCol != -1
            // Parse this complete problem block
            parseBlock(G, startCol, j - 1, operations)
            startCol = -1

    // Process the final block if the grid does not
    // end with a completely blank column
    if startCol != -1
        parseBlock(G, startCol, C - 1, operations)

    return operations
```

Assume that each math problem is stored as an `Operation` containing an operator type and a list of numbers.

```text
parseBlock(G, startCol, endCol, operations)

    // Allocate a new math problem
    currentOperation = new empty Operation
    R = G.rows

    for i = 0 to R - 1
        rowSegment = ""

        for j = startCol to endCol
            rowSegment = rowSegment + G[i, j]

        hasOperator = false

        for each character c in rowSegment
            if c == '+' or c == '*'
                currentOperation.type = c
                hasOperator = true
                break

        if hasOperator == false
            currentNumber = 0
            readingNumber = false

            for each character c in rowSegment
                if isDigit(c)
                    currentNumber =
                        (currentNumber × 10) + (c - '0')

                    readingNumber = true

                else if readingNumber == true
                    currentOperation.numbers.push_back(currentNumber)
                    currentNumber = 0
                    readingNumber = false

            // Store a number that reaches the end of the row
            if readingNumber == true
                currentOperation.numbers.push_back(currentNumber)

    // Store only valid math problems
    if currentOperation.numbers.size > 0
        operations.push_back(currentOperation)
```

## Pseudocode — Part 1 Solution

Once the data has been stored inside the parsed structures, we perform the arithmetic reduction and sum the results.

```text
// O is an Operation containing the fields
// type and numbers
computeOperation(O)
    if O.numbers.size == 0
        return 0

    if O.type == '+'
        result = 0
    else
        result = 1

    for i = 0 to O.numbers.size - 1
        if O.type == '+'
            result = result + O.numbers[i]
        else
            result = result × O.numbers[i]

    return result
```

Assume that `O` contains all the math problems after the input has been parsed correctly.

```text
solvePart1(G)
    O = readAndParseGrid(G)
    sum = 0

    for i = 0 to O.size - 1
        sum = sum + computeOperation(O[i])

    return sum
```

## Complexity

Let:

* `R` be the total number of rows in the worksheet;
* `C` be the maximum right-padded row width;
* `K` be the total number of integers extracted from all the problem blocks.

Reading the worksheet, calculating the maximum width, and padding the shorter rows requires `Θ(R × C)` time.

Scanning the columns to find the completely blank boundaries also requires `Θ(R × C)` time in the worst case, since each column may be checked across all `R` rows.

Processing the individual problem blocks examines only cells contained within their non-overlapping column ranges. Across all the blocks, this is bounded by `Θ(R × C)`.

The arithmetic reduction processes each extracted integer exactly once and therefore costs:

```text
Θ(K)
```

Since all the integers are extracted from the worksheet, `K = O(R × C)`.

The overall time complexity is therefore:

```text
T(R, C) = Θ(R × C)
```

### Space Complexity

To obtain a rectangular layout with safe indexing, the normalized padded worksheet is kept in memory.

It requires:

```text
Θ(R × C)
```

space.

The parsed operations and their numbers are also bounded by the amount of data contained in the worksheet, so they require at most `O(R × C)` space.

Therefore, the total auxiliary space complexity is:

```text
Θ(R × C)
```

## Part 2

In Part 2, we learn that the rules of cephalopod math are different.

The equations are read by columns, from right to left.

Each individual column within a problem block forms its own multi-digit number, with the most significant digit at the top and the least significant digit at the bottom, immediately above the operator row.

## Idea

Addition and multiplication are commutative:

```text
A + B = B + A
A × B = B × A
```

Therefore, the order in which we extract and process the vertical numbers does not affect the final arithmetic result.

However, to follow the right-to-left specification of cephalopod math directly, we adjust the sub-grid scanning loops.

Phases 1 and 2 from Part 1 remain unchanged, while Phase 3 changes as follows:

1. We inspect the bottom-most row, `R - 1`, within the block boundaries `[startCol, endCol]` to extract the mathematical operator.
2. Instead of scanning the block row by row, we loop through its columns from right to left, from `endCol` down to `startCol`.
3. For each column, we scan vertically from the top row down to the row immediately above the operator, from `0` to `R - 2`.
4. As we move down a column, we accumulate its digits using the same decimal-building logic used in Part 1.
5. If a column contains no digits, the `readingNumber` flag remains `false`, so the column is ignored.

Since the digits are stacked vertically with the highest place value at the top, scanning from top to bottom constructs each number correctly.

## Pseudocode

Phases 1 and 2 remain unchanged, so only the Phase 3 parser needs to be adapted.

```text
parseBlockPart2(G, startCol, endCol, operations)
    currentOperation = new empty Operation
    R = G.rows

    // Scan the bottom row for the mathematical operator
    for j = startCol to endCol
        if G[R - 1, j] == '+' or G[R - 1, j] == '*'
            currentOperation.type = G[R - 1, j]
            break

    // Scan columns from right to left
    // to construct the vertical numbers
    for j = endCol downto startCol
        currentNumber = 0
        readingNumber = false

        // Scan vertically from the top row to the row
        // immediately above the operator row
        for i = 0 to R - 2
            c = G[i, j]

            if isDigit(c)
                currentNumber =
                    (currentNumber × 10) + (c - '0')

                readingNumber = true

        // Store the number if this column contained digits
        if readingNumber == true
            currentOperation.numbers.push_back(currentNumber)

    // Store the operation if it is valid
    if currentOperation.numbers.size > 0
        operations.push_back(currentOperation)
```

The same `computeOperation` procedure from Part 1 can then be used to evaluate every parsed operation.

## Complexity

Part 2 performs the same grid normalization and problem-boundary scan as Part 1.

Every cell within the problem blocks is inspected a constant number of times, so the total time complexity remains:

```text
T(R, C) = Θ(R × C)
```

The padded worksheet and parsed operations require:

```text
Θ(R × C)
```

auxiliary space.
