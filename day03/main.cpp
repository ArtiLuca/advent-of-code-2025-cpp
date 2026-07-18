#include "day03.h"

#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

int main() {

    try {
        Escalator lobby{"input.txt"};
        lobby.readPuzzleInput();

        std::cout << "--- Day 3: Lobby ---\n";
        std::cout << "Part 1: " << lobby.solvePart1() << '\n';
        std::cout << "Part 2: " << lobby.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;

}
