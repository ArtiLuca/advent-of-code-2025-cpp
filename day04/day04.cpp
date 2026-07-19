#include "day04.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <utility>
#include <queue>


// =============== PUZZLE INPUT & DEBUGGING ===============

const int Forklift::dr[] = {1, 1, 1, 0, 0, -1, -1, -1};
const int Forklift::dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

void Forklift::readPuzzleInput() {

    grid.matrix.clear();

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    std::string line;
    bool isFirstLine = true;
    std::string topBottomPadding = "";

    while (std::getline(file,line)) {
        if (line.empty()) continue;

        // dynamic padding only first iteration
        // works because our grid is perfect square (if not, would need some adjustments)
        if (isFirstLine) {
            int paddedLength = line.length() + 2;
            // push top padding to grid
            topBottomPadding = std::string(paddedLength, '.');
            grid.matrix.push_back(topBottomPadding);
            isFirstLine = false;
        }

        // wrap original line with '.' on both ends
        std::string paddedLine = '.' + line + '.';
        grid.matrix.push_back(paddedLine);
    }

    // push bottom padding to grid
    if (!topBottomPadding.empty()) {
        grid.matrix.push_back(topBottomPadding);
    }

    file.close();
}

void Grid::printGrid(bool showPadding) const {

    if (matrix.empty()) return;

    int numCols = matrix[0].length();
    int numRows = matrix.size();
    int startRow = 0;
    int startCol = 0;

    if (!showPadding) {
        numCols--;
        numRows--;
        startRow++;
        startCol++;
    }

    for (int i = startRow; i < numRows; ++i) {
        for (int j = startCol; j < numCols; ++j) {
            std::cout << matrix[i][j];
        }
        std::cout << "\n";
    }
}


// =============== PART 1 ===============

int Forklift::solvePart1() const {

    // guard 
    if (grid.matrix.empty()) {
        throw std::runtime_error("Grid is empty.");
    }

    int totalCount = 0;
    int rows = grid.matrix.size();
    int cols = grid.matrix[0].length();

    // scan out effective grid, ignoring padding
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            
            // only evaluate cells with paper rolls
            if (grid.matrix[i][j] == PAPER_ROLL) {

                int count = 0;
                // count adjacent positions
                for (int k = 0; k < 8; ++k) {
                    if (grid.matrix[i+dr[k]][j+dc[k]] == PAPER_ROLL)
                        count++;
                }

                if (count < 4)
                    totalCount++;
            }
        }
    }
    
    return totalCount;
}

// =============== PART 2 ===============

int Forklift::solvePart2() const {

    // guard
    if (grid.matrix.empty()) {
        throw std::runtime_error("Grid is empty.");
    }

    // create copy of grid
    Grid copy = grid;

    int totalRemoved = 0;
    int rows = copy.matrix.size();
    int cols = copy.matrix[0].length();

    // create queue using pair<int,int> as coordinates of a cell (i,j)
    std::queue<std::pair<int,int>> Q;

    // scan out effective grid to find initial removable paper rolls
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {

            if (copy.matrix[i][j] == PAPER_ROLL) {
                int count = 0;
                for (int k = 0; k < 8; ++k) {
                    if (copy.matrix[i + dr[k]][j + dc[k]] == PAPER_ROLL) 
                        count++;
                }
                
                if (count < 4) 
                    Q.push({i,j});
            }
        }
    }

    // process cascading removals 
    while (!Q.empty()) {
        
        int r = Q.front().first;
        int c = Q.front().second;
        Q.pop();

        // if removed previously, we skip
        if (copy.matrix[r][c] != PAPER_ROLL) 
            continue;

        // remove paper roll by marking in copy
        copy.matrix[r][c] = REMOVED;
        totalRemoved++;

        // explore neighbours to check if any adjacent paper rolls can now be removed
        for (int k = 0; k < 8; ++k) {
            
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (copy.matrix[nr][nc] == PAPER_ROLL) {
                int neighbourCount = 0;

                // count paper rolls around current neighbour
                for (int nk = 0; nk < 8; ++nk) {
                    if (copy.matrix[nr + dr[nk]][nc + dc[nk]] == PAPER_ROLL) 
                        neighbourCount++;
                }

                // if this neighbour is now newly removable, we push to queue
                if (neighbourCount < 4)
                    Q.push({nr,nc}); 
            }
        }
    }

    return totalRemoved;
}