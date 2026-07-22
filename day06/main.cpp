#include "day06.h"

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        CephalopodMath math{"input.txt"};
        
        std::cout << "--- Day 6: Trash Compactor ---\n";
        
        math.readPuzzleInput(false);
        std::cout << "Part 1: " << math.sumOperationResults() << '\n';
        
        math.readPuzzleInput(true);
        std::cout << "Part 2: " << math.sumOperationResults() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}