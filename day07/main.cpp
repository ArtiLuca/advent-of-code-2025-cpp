#include "day07.h"

#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Teleporter teleporter{"input.txt"};
        teleporter.readPuzzleInput();

        std::cout << "--- Day 7: Teleporter Hub ---\n";
        std::cout << "Part 1: " << teleporter.solvePart1() << '\n';
        std::cout << "Part 2: " << teleporter.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}