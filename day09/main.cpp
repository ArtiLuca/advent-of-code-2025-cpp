#include "day09.h"

#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        MovieTheater movies{"input.txt"};
        movies.readPuzzleInput();

        std::cout << "--- Day 9: Movie Theater ---\n";
        std::cout << "Part 1: " << movies.solvePart1() << '\n';
        std::cout << "Part 2: " << movies.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
