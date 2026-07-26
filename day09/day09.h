#pragma once

#include <string>
#include <vector>

struct Point {

    long long x;
    long long y;
};

struct LineSegment {

    long long x1, y1;
    long long x2, y2;
};


class MovieTheater {

public:

    MovieTheater(const std::string& input) : puzzleInput(input) {}

    void readPuzzleInput();
    long long solvePart1();
    long long solvePart2();


private:

    std::string puzzleInput;
    std::vector<Point> points;

    // validity check helper for Part 2
    bool isValidRectangle(const Point& p1, const Point& p2, const std::vector<LineSegment>& edges);

};