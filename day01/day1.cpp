#include "day1.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>

// =============== PUZZLE INPUT ===============

void Safe::readPuzzleInput() {

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    else {
        
        // Read all rotations from file
        std::string line;
        while (std::getline(file, line)) {
            Rotation rot;
            rot.dir = line.at(0);
            rot.val = std::stoi(line.substr(1));
            rotations.push_back(rot);
        }
        numRotations = (int) rotations.size();
    }
}    

    
// =============== PART 1 ===============    

int Rotation::rotateDial(int start) const {

    // Validity check
    if (dir != 'R' && dir != 'L') {
        throw std::runtime_error("Not a valid rotation!");
    }

    // Perform base modulo-100 turn depending on direction
    int next = (dir == 'R') ? (start + val) % 100 : (start - val) % 100;
    
    // If needed, turn negative positions into the positive 0-99 interval
    while (next < 0) {
        next += 100;
    }

    // Return finalized remainder within 0-99 interval
    return next % 100;
}

int Safe::solvePart1(bool comment) {

    // start at current dial (50)
    int currentDialValue = currentDial;
    int zeroCount = 0;

    // perform each rotation
    for (const auto& rot : rotations) {

        // rotate dial and get next value
        int nextDialValue = rot.rotateDial(currentDialValue);
        // update if we land on 0
        if (nextDialValue == 0)  zeroCount++;
        // update current dial
        currentDialValue = nextDialValue;
    }

    if (comment) std::cout << "Password is: " << zeroCount << "\n";
    return zeroCount;
}


// =============== PART 2 ===============

int Rotation::countZeroCrossings(int start) const {

    // Validity check
    if (dir != 'R' && dir != 'L') {
        throw std::runtime_error("Not a valid rotation!");
    }

    // Case: Right
    if (dir == 'R') {
        // integer division returns how many times we cross or land on zero neatly
        return (start + val) / 100;
    }
    
    // Case: Left (a bit more steps to keep in mind)
    else {
        // we determine how many steps it takes to reach 0 going backwards
        // if we're at 0, we need exactly 100
        int distanceFromZero = (start == 0) ? 100 : start;
        
        // if we don't reach zero, we return
        if (val < distanceFromZero) return 0;

        // otherwise, we count 1 for hitting the first zero,
        // and divide the remaining distance by 100 to count any extra crossings
        int nextDistance = val - distanceFromZero;
        return 1 + (nextDistance / 100); 
    }

}

int Safe::solvePart2(bool comment) {
    
    // like part 1
    int currentDialValue = currentDial;
    int zeroCount = 0;

    for (const auto& rot : rotations) {

        // like part 1, but we first count the zero-crossings
        int numCrossings = rot.countZeroCrossings(currentDialValue);
        // update zero-count
        zeroCount += numCrossings;
        // update current dial value
        currentDialValue = rot.rotateDial(currentDialValue);
    }

    if (comment) std::cout << "Password is " << zeroCount << "\n";
    return zeroCount;
}
