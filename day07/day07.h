#pragma once

#include <string>
#include <vector>

inline constexpr char ENTRY_POINT = 'S';
inline constexpr char EMPTY_SPACE = '.';
inline constexpr char SPLITTER = '^';

/* Represents the tachyon manifold manual (diagram )*/
struct TachyonManifold {

    std::vector<std::string> diagram;
};

class Teleporter {

public: 
    
    Teleporter(const std::string& input) : puzzleInput(input) {}

    // reads input into 2D grid
    void readPuzzleInput();

    // runs layer-queue wavefront cascade simulation and returns total splits
    long long solvePart1();

    long long solvePart2();

private:

    std::string puzzleInput;
    TachyonManifold grid;
};