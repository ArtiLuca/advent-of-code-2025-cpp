#pragma once

#include <string>
#include <vector>

/* Point holds 3D coordinates of junction boxes */
struct Point {

    long long x;
    long long y;
    long long z;
};

/* tracks indexes pointing back to the vector of junction boxes */
struct Edge {

    int a; // index of first box
    int b; // index of second box
    long long squaredDistance;
};


/* disjoint set structure for circuit isolation tracking */
struct DisjointSet {

    // constructor
    DisjointSet(int n);
    // find parent (circuit) of a given junction box
    int find(int i);
    // merge two entire circuits togethere
    void merge(int i, int j);

    // returns boolean value if merge was successful
    bool mergePart2(int i, int j);

    std::vector<int> parent;
    std::vector<int> size;
};


class Playground {

public:

    Playground(const std::string& input) : puzzleInput(input) {}

    void readPuzzleInput();
    long long solvePart1();
    long long solvePart2();

    long long getSquaredDistance(const Point& p1, const Point& p2);


private:

    std::string puzzleInput;
    std::vector<Point> junctionBoxes;
    
    std::vector<Edge> allEdges;

};