#include "day07.h"

#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <exception>
#include <stdexcept>
#include <algorithm>

// =============== PUZZLE INPUT ===============

void Teleporter::readPuzzleInput() {

    // clear previous result
    grid.diagram.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    std::string line;
    while (std::getline(file, line)) {

        // skip any empty lines at the beginning (if present)
        if (!line.empty()) {
            grid.diagram.push_back(line);
        }
    }
    file.close();
}

// =============== PART 1 ===============

long long Teleporter::solvePart1() {

    // guard 
    if (grid.diagram.empty())
        return 0;

    int rows = grid.diagram.size();
    int cols = grid.diagram[0].length();

    long long totalSplits = 0;
    int startRow = -1;
    int startCol = -1;

    // scan grid to find entry point
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid.diagram[r][c] == ENTRY_POINT) {
                startRow = r;
                startCol = c;
                break;
            }
        }
        
        if (startRow != -1)
            break;
    }
    
    if (startRow == -1)
        return 0;

    // initialize active column tracking set with the starting column
    std::set<int> activeCols;
    activeCols.insert(startCol);

    // cascade downards row-by-row
    for (int r = startRow + 1; r < rows; ++r) {

        // early exit (if no beams from previous layer, stop tracking)
        if (activeCols.empty())
            break;

        std::queue<int> rowQueue;
        std::set<int> rowVisited;
        std::set<int> splittersEncountered;

        // load all active propagating columns into the row queue workspace
        for (int c : activeCols)
            rowQueue.push(c);

        // process horizontal cascades on the current tow layer 
        while (!rowQueue.empty()) {

            // fetch front tracking index from queue
            int c = rowQueue.front();
            rowQueue.pop();

            // skip out-of-bounds or coordinates already tracked on this row 
            if (c < 0 || c >= cols || rowVisited.count(c) > 0)
                continue;

            // store index to prevent redundancy
            rowVisited.insert(c);

            // Case A: beam encounters a splitter
            if (grid.diagram[r][c] == SPLITTER) {

                // ensure each splitter is encountered only once per row layer
                if (splittersEncountered.count(c) == 0) {
                    splittersEncountered.insert(c);
                    totalSplits++;

                    // push lateral neighbours to continue same-row chain reaction
                    rowQueue.push(c-1);
                    rowQueue.push(c+1);
                }
            }
        }
        
        // Case B: Beam encounters empty space (ENTRY_POINT or EMPTY_POINT)
        // No lateral splits are triggered; the beam naturally passes through 
        // and is captured by rowVisited to propagate downwards in Step 3.

        // filter out splitters to prepare the downard beam propagation
        std::set<int> nextRowCols;
        for (int c : rowVisited) {
            if (grid.diagram[r][c] != SPLITTER) {
                nextRowCols.insert(c);
            }
        }

        // advance simulation tracking context to next row layer
        activeCols = std::move(nextRowCols);
    }    

    return totalSplits;
}

// =============== PART 2 ===============

long long Teleporter::solvePart2() {
    
    // guard 
    if (grid.diagram.empty())
        return 0;

    int rows = grid.diagram.size();
    int cols = grid.diagram[0].length();

    int startRow = -1;

    // allocate a array to track timelines for the previous row
    std::vector<long long> prevRow(cols, 0);

    // locate entry point 'S' and initialize base case
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; c++) {
            
            if (grid.diagram[r][c] == ENTRY_POINT) {
                prevRow[c] = 1;
                startRow = r;
                break;
            }
        }
        if (startRow != -1) 
            break;
    }
    if (startRow == -1)
        return 0;

    // process grid layer-by-layer using DP recurrence    
    for (int i = startRow + 1; i < rows; ++i) {

        // allocate tracking for the current row
        std::vector<long long> currRow(cols, 0);

        for (int j = 0; j < cols; ++j) {

            if (grid.diagram[i][j] == SPLITTER) {
                currRow[j] = 0;
            }
            
            else if (grid.diagram[i][j] == EMPTY_SPACE) {
            
                long long sum = 0;

                // check directly above
                if (grid.diagram[i-1][j] != SPLITTER) 
                    sum += prevRow[j];
                
                // check diagonal above-left for a splitter redirecting right 
                if (j > 0 && grid.diagram[i][j-1] == SPLITTER) 
                    sum += prevRow[j-1];

                // check diagonal above-right for a splitter redirecting left   
                if (j < cols - 1 && grid.diagram[i][j+1] == SPLITTER)
                    sum += prevRow[j+1];

                currRow[j]= sum;    
            }
        }

        // advance DP state
        prevRow = std::move(currRow);
    }

    // sum all timelines that successfully made it to the bottom row
    long long totalTimelines = 0;
    for (int j = 0; j < cols; ++j)
        totalTimelines += prevRow[j];

    return totalTimelines;    
}