#pragma once

#include <string>
#include <vector>


/* Representa an interval-range of product IDs */
struct Interval {

    long long start;
    long long end;
    
    // returns true if ID is valid
    bool isValidID(long long current) const;
    // returns sum of invalid IDs in interval
    long long sumInvalidIDs() const;

    // returns true if ID is valid (with part 2 logic)
    bool isValidIDPart2(long long current) const;
    // returns sum of invalid IDs in interval (with part 2 logic)
    long long sumInvalidIDsPart2() const;
};


/* Represents the gift shop and interval-range product IDs */
class GiftShop {

public:

    // constructor 
    GiftShop(const std::string& input) : puzzleInput(input) {}
    
    // reads puzzle input from file, storing inteval ranges in vector
    void readPuzzleInput();

    // checks given interval, returning sum of invalid IDs found
    long long checkInterval(size_t i) const;
    // checks all interval ranges and returns total sum of invalid IDs found
    long long solvePart1(bool comment = false) const;

   
    // checks given interval, returning sum of invalid IDs found (with part 2 logic)
    long long checkIntervalPart2(size_t i) const;
    // checks all interval ranges and returns total sum of invalid IDs found (with part 2 logic)
    long long solvePart2(bool comment = false) const;


private:

    std::string puzzleInput;
    std::vector<Interval> ranges;

};