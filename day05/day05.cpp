#include "day05.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <exception>
#include <stdexcept>

// =============== PUZZLE INPUT  ===============

void Cafeteria::readPuzzleInput() {

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    std::string line;
    bool readingRanges = true;

    while (std::getline(file, line)) {

        // A single blank line marks the transition boundary between data sets
        if (line.empty()) {
            readingRanges = false;
            continue;
        }

        if (readingRanges) {
            size_t dash = line.find('-');
            if (dash != std::string::npos) {
                Range r;
                r.start = std::stoull(line.substr(0,dash));
                r.end = std::stoull(line.substr(dash + 1));
                ranges.push_back(r);
            }
        }
        else {
            products.push_back(std::stoull(line));
        }
    }

    file.close();
}


// =============== HELPERS ===============

bool Cafeteria::compareRanges(const Range& r1, const Range& r2) {

    // Sort comparator sorting ranges strictly by ascending lower bound limits
    return r1.start < r2.start;
}


void Cafeteria::prepareRanges() {
    
    if (ranges.empty()) return;

    // Phase 1: sort ranges in-place using IntroSort
    std::sort(ranges.begin(), ranges.end(), Cafeteria::compareRanges);

    // Phase 2: cleanup if needed, then collapse overlaps
    mergedRanges.clear();
    for (const auto& current : ranges) {

        // If current range falls entirely past previous upper limit, store it as a new distinct block
        if (mergedRanges.empty() || current.start > mergedRanges.back().end)
            mergedRanges.push_back(current);
        else {
            // Otherwise, combine them by stretching the upper edge of the last tracked block
            mergedRanges.back().end = std::max(mergedRanges.back().end, current.end);
        }
            
    }
}

bool Cafeteria::binarySearch(unsigned long long id) const{

    int p = 0;
    int r = mergedRanges.size() - 1;

    // Iterative D&C Search loop over disjoint array bounds
    while (p <= r) {
        int q = p + (r - p) / 2;

        // case 1: ID falls perfectly inside current collapsed interval
        if (id >= mergedRanges[q].start && id <= mergedRanges[q].end)
            return true;

        // case 2: ID is strictly to the left, discard right
        else if (id < mergedRanges[q].start)
            r = q - 1;    

        // case 3: ID is strictly to the right, discard left
        else 
            p = q + 1;     
    }

    // otherwise
    return false;
}



// =============== PART 1 ===============

long long Cafeteria::solvePart1() {

    // Setup merged ranges
    prepareRanges();

    // Loop through standalone list and match entries via Binary Search
    long long count = 0;
    for (unsigned long long id : products) {
        if (binarySearch(id)) 
            count++;
    }

    return count;

}


// =============== PART 2 ===============

long long Cafeteria::solvePart2() {

    // setup
    prepareRanges();

    // Since intervals are strictly disjoint, total points equals the sum of inclusive dimensions
    unsigned long long count = 0;
    for (const auto& mr : mergedRanges) 
        count += (mr.end - mr.start + 1);
        
    return count;    
}

