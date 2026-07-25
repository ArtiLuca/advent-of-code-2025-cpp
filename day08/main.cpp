#include "day08.h"

#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Playground playground{"input.txt"};
        playground.readPuzzleInput();

        std::cout << "--- Day 8: Playground ---\n";
        std::cout << "Part 1: " << playground.solvePart1() << '\n';
        std::cout << "Part 2: " << playground.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}