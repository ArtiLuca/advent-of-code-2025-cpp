#include "day02.h"

#include <string>
#include <exception>
#include <iostream>


using namespace std;

int main() {

    try {
        GiftShop shop{"input.txt"};
        shop.readPuzzleInput();

        std::cout << "--- Day 2: Gift Shop ---\n";
        std::cout << "Part 1: " << shop.solvePart1() << '\n';
        std::cout << "Part 2: " << shop.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

