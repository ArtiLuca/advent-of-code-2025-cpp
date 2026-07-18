#pragma once

#include <string>
#include <vector>

/* Represents a single bank of batteries */
struct Bank {
    
    std::string batteries;

    // two-digit joltage
    int findJoltage() const;
    // twelve-digit joltage
    long long findJoltagePart2() const;
};



/* Represents the escalator to power up */
class Escalator {

public:

    Escalator(const std::string& input) : puzzleInput(input) {}
    void readPuzzleInput();

    // solves part 1 using two-scan approach
    int solvePart1() const;
    // solves part 2 using greedy approach
    long long solvePart2() const;

private:

    std::string puzzleInput;
    std::vector<Bank> banks;

};