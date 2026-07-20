#include "day05.h"

#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Cafeteria cafe{"input.txt"};
        cafe.readPuzzleInput();

        std::cout << "--- Day 5: Cafeteria ---\n";
        std::cout << "Part 1: " << cafe.solvePart1() << '\n';
        std::cout << "Part 2: " << cafe.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
