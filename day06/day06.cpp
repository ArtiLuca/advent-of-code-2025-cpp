#include "day06.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <cctype>
#include <algorithm>


// =============== PUZZLE INPUT ===============

void CephalopodMath::readPuzzleInput(bool part2) {

    operations.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // to make things easier we want all rows to be the same length
    // so we allocate a temporary copy of the 2D grid read from input 
    std::vector<std::string> paddedGrid;
    std::string line;
    size_t maxLength = 0;

    // Phase 1: read and apply padding so all rows have equal length
    while (std::getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        paddedGrid.push_back(line);
        maxLength = std::max(maxLength, line.length());
    }
    file.close();

    // guard
    if (paddedGrid.empty())
        return;

    // apply padding
    for (std::string& row : paddedGrid) {
        if (row.length() < maxLength) {
            row.append(maxLength - row.length(), ' ');
        }
    }
    
    int rows = paddedGrid.size();
    int cols = maxLength;

    // Phase 2: find horizontal column boundaries for each problem block
    int startCol = -1;

    for (int j = 0; j < cols; ++j) {

        // check if the current column contains any visible characters
        bool columnHasContent = false;
        for (int i = 0; i < rows; ++i) {

            if (paddedGrid[i][j] != ' ') {
                columnHasContent = true;
                break;
            }
        }

        if (columnHasContent && startCol == -1) {
            // we found the starting boundary of a problem bock
            startCol = j;
        }
        else if (!columnHasContent && startCol != -1) {
            
            // we found a trailing space column: parse complete block [startCol, j-1]
            // using dedicated helper, depending on booealn flag 'part2'

            if (part2) {
                parseBlockPart2(paddedGrid, startCol, j-1);
            }
            else {
                parseBlockPart1(paddedGrid, startCol, j-1);
            }
            // reset tracker
            startCol = -1;
        }
    }

    // process trailing block if the input grid doesnt end with spaces
    if (startCol != -1) {

        // use boolean flag to choose parsing logic
        if (part2) {
            parseBlockPart2(paddedGrid, startCol, cols - 1);
        }
        else {
            parseBlockPart1(paddedGrid, startCol, cols - 1);
        }
    }
}

// =============== PART 1 ===============

void CephalopodMath::parseBlockPart1(const std::vector<std::string>& grid, int startCol, int endCol) {

    Operation currentOperation{};
    int rows = grid.size();

    // iterate vertically down the problem slice row by row
    for (int i = 0; i < rows; ++i) {
        std::string rowSlice = "";

        // extract exact sub-string segment for this row
        for (int j = startCol; j <= endCol; ++j) {
            rowSlice += grid[i][j];
        }    
        
        // check if this row contains math operator at the bottom of the column
        //size_t operatorPos = rowSegment.find_first_of(" +*");
        bool hasOperator = false;
        for (char c : rowSlice) {
            if (c == ADDITION || c == MULTIPLICATION) {
                currentOperation.type = c;
                hasOperator = true;
                break;
            }
        }

        // it not operator row, scan for numbers horizontally
        if (!hasOperator) {

            long long currentNumber = 0;
            bool readingNumber = false;

            for (char c : rowSlice) {
            
                if (std::isdigit(static_cast<unsigned char>(c))) {
                    currentNumber = currentNumber * 10 + (c - '0');
                    readingNumber = true;
                }
                else if (readingNumber) {
                    currentOperation.numbers.push_back(currentNumber);
                    currentNumber = 0;
                    readingNumber = false;
                }
            }

            if (readingNumber) {
                currentOperation.numbers.push_back(currentNumber);
            }
        }
    }
    
    // append any valid extract problems
    if (!currentOperation.numbers.empty()) {
        operations.push_back(currentOperation);
    } 
}

long long Operation::calculateOperation() const {

    if (numbers.empty())
        return 0;

    long long result = 0;
    if (type == MULTIPLICATION) 
        result = 1;
        
    for (const auto& num : numbers) {
        if (type == ADDITION)
            result += num;
        else if (type == MULTIPLICATION) 
            result *= num;    
    }
    return result;
}

long long CephalopodMath::sumOperationResults() const {

    long long sum = 0;

    for (const auto& op : operations)
        sum += op.calculateOperation();
    return sum;
}

// =============== PART 2 ===============

void CephalopodMath::parseBlockPart2(const std::vector<std::string>& grid, int startCol, int endCol) {

    Operation currentOperation{};
    int rows = grid.size();

    // find operator at bottom row of this block slice
    // which willbe at grid[rows - 1] within the block's columns
    for (int j = startCol; j <= endCol; ++j) {

        char c = grid[rows-1][j];

        if (c == ADDITION || c == MULTIPLICATION) {
            currentOperation.type = c;
            break;
        }
    }

    // scan columns right-to-left to construct vertical numbers
    for (int j = endCol; j >= startCol; --j) {

        long long currentNumber = 0;
        bool readingNumber = false;

        // iterate vertically from top-row downto row jut above operator
        for (int i = 0; i < rows - 1; ++i) {

            char c = grid[i][j];

            if (std::isdigit(static_cast<unsigned char>(c))) {
                currentNumber = currentNumber * 10 + (c - '0');
                readingNumber = true;
            }
        }

        // if this column contained digits, store it as complete number
        if (readingNumber) {
            currentOperation.numbers.push_back(currentNumber);
        }
    }

    // add problem to operations list, if valid
    if (!currentOperation.numbers.empty()) {
        operations.push_back(currentOperation);
    }
}
