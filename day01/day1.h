#pragma once

#include <string>
#include <vector>
#include <iostream>

/* Represents a single roation of the dial */
struct Rotation {

    char dir;
    int val;
        
    int rotateDial(int start) const; 
    int countZeroCrossings(int start) const;
    
};


/* Represents the safe with the rotations performed on its dial */
class Safe {

    public:

    Safe(const std::string& input) : 
        puzzleInput(input), 
        currentDial(50) {}

    void readPuzzleInput();
    int solvePart1(bool comment = false);
    int solvePart2(bool comment = false);

private:

    std::string puzzleInput;
    int currentDial;
    std::vector<Rotation> rotations;
    int numRotations;
};