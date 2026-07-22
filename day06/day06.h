#pragma once

#include <string>
#include <vector>

// math operators
// const char ADDITION = '+';
// const char MULTIPLICATION = '*'; 
inline constexpr char ADDITION = '+';
inline constexpr char MULTIPLICATION = '*';


/* Represents a single math problem with numbers and operator type */
struct Operation {

    std::vector<long long> numbers;
    char type = '\0';

    // computes math problem using assigned operator and stored numbers
    long long calculateOperation() const;
};

class CephalopodMath {

public:

    CephalopodMath(const std::string& input) : puzzleInput(input) {}

    // reads input file into grid, applying right-padding and mapping problem boundaries
    void readPuzzleInput(bool part2 = false);

    // solves part 1 by computing all operations and summing them up
    long long sumOperationResults() const;

private:

    // private helpers
    // isolates a single boundedcolumn sub-grid slice and extracts its rows horizontally
    void parseBlockPart1(const std::vector<std::string>& grid, int startCol, int endCol);
    void parseBlockPart2(const std::vector<std::string>& grid, int startCol, int endCol);

    std::string puzzleInput;
    std::vector<Operation> operations;

};