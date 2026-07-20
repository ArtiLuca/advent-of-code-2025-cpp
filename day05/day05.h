#pragma once

#include <string>
#include <vector>
#include <algorithm>

struct Range {

    unsigned long long start;
    unsigned long long end;
};

class Cafeteria {

public:

    Cafeteria(const std::string& input) : puzzleInput(input) {}

    void readPuzzleInput();

    long long solvePart1();
    long long solvePart2();

private:

    bool binarySearch(unsigned long long id) const;
    static bool compareRanges(const Range& r1, const Range& r2);
    void prepareRanges();

    std::string puzzleInput;
    std::vector<Range> ranges;
    std::vector<Range> mergedRanges;
    std::vector<unsigned long long> products;
};