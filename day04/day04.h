#pragma once

#include <string>
#include <vector>

// to improve readibility
const char PAPER_ROLL = '@';
const char REMOVED = 'x';

/* Represents the grid on which the forklift operates on */
struct Grid {

    std::vector<std::string> matrix;

    // debugging
    void printGrid(bool showPadding = false) const;
};

/* Represents the forklift */
class Forklift {

    public:

    // constructor
    Forklift(const std::string& input) : puzzleInput(input) {}
    
    // reads puzzle input and adds extra padding 
    void readPuzzleInput();

    // solves part 1
    int solvePart1() const;
    // solves part 2
    int solvePart2() const;

    static const int dr[];
    static const int dc[];

    // getter for grid
    const Grid& getGrid() const { return grid; }

    private:

    std::string puzzleInput;
    Grid grid;    

};