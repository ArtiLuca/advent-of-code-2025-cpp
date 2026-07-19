#include "day04.h"

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Forklift lift{"input.txt"};
        lift.readPuzzleInput();

        std::cout << "--- Day 4: Printing Department ---\n";
        std::cout << "Part 1: " << lift.solvePart1() << '\n';
        std::cout << "Part 2: " << lift.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}