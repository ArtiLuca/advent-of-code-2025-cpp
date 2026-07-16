#include <string>
#include <fstream>
#include "day1.cpp"

using namespace std;

int main() {

  try {
        Safe safe{"input_1.txt"};
        safe.readPuzzleInput();

        std::cout << "--- Day 1: Secret Entrance ---\n";
        std::cout << "Part 1: " << safe.solvePart1() << '\n';
        std::cout << "Part 2: " << safe.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;

}
