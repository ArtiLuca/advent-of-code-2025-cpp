#include "day08.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <numeric>
#include <algorithm>


// =============== PUZZLE INPUT ===============

void Playground::readPuzzleInput() {

    // clear previous results
    junctionBoxes.clear();
    allEdges.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read input file line by line, parsine 3D coordinates of junction boxes
    long long x, y, z;
    char comma1, comma2;

    // loop ends when reaches EOF
    while (file >> x >> comma1 >> y >> comma2 >> z) {
        // validity check as guard
        if (comma1 == ',' && comma2 == ',') {
            Point p;
            p.x = x; p.y = y; p.z = z;
            junctionBoxes.push_back(p);
        }
    }

    file.close();

    int n = junctionBoxes.size();

    // memory pre-allocation optimization
    allEdges.reserve((n * (n-1)) / 2);

    // generate all unique pairs of connections
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long squaredDist = getSquaredDistance(junctionBoxes[i], junctionBoxes[j]);
            allEdges.push_back({i, j, squaredDist});
        }
    }

    // sort all pairs from shortest to longest distance
    std::sort(allEdges.begin(), allEdges.end(), [](const Edge& left, const Edge& right) {
        return left.squaredDistance < right.squaredDistance;
    });
}


// =============== HELPERS/DATA STRUCTURES ===============

DisjointSet::DisjointSet(int n) {

    parent.resize(n);
    // initially populates sequentially increasing values (0,1,2,3...)
    std::iota(parent.begin(), parent.end(), 0);
    // each junction box initially has a size of 1
    size.assign(n, 1); 
}

long long Playground::getSquaredDistance(const Point& p1, const Point& p2) {

    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    long long dz = p1.z - p2.z;
    return (dx * dx) + (dy * dy) + (dz * dz); 
}

int DisjointSet::find(int i) {

    if (parent[i] == i) 
        return i;
    
    // path compression -> to speed up future lookups
    return parent[i] = find(parent[i]);
}

void DisjointSet::merge(int i, int j) {

    // find parents of junction boxes
    int root_i = find(i);
    int root_j = find(j);

    // if parents differ we want to merge the smaller one into the larger one
    if (root_i != root_j) {
        
        if (size[root_i] < size[root_j]) {
            // and also update so i is always the largest
            std::swap(root_i, root_j);
        }

        // update parent of j and resize i in order that it can hold j
        parent[root_j] = root_i;
        size[root_i] += size[root_j];
    }

    // if root_i == root_j
    // if they are already in the same circuit, nothing happens
}

bool DisjointSet::mergePart2(int i, int j) {

    // same as original 'merge'
    int root_i = find(i);
    int root_j = find(j);

    if (root_i != root_j) {
        
        if (size[root_i] < size[root_j]) {
            std::swap(root_i, root_j);
        }
        
        parent[root_j] = root_i;
        size[root_i] += size[root_j];
        // merge operation was successful
        return true;
    }

    // otherwise, merge operation was not successful
    return false;
}


// =============== PART 1 ===============

long long Playground::solvePart1() {

    int n = junctionBoxes.size();

    // guard 
    if (n < 3) {
        throw std::runtime_error("Error: Not enough coordinates to find 3 circuits.");
    }

    // 3. Initialize the Disjoint Set structure and connect the top 1000 pairs
    DisjointSet dsu(n);
    int numConnections = std::min(1000, (int)this->allEdges.size());

    for (int k = 0; k < numConnections; ++k) {
        dsu.merge(this->allEdges[k].a, this->allEdges[k].b);
    }

    // 4. Extract and process the final unique circuit sizes
    std::vector<long long> circuitSizes;
    for (int i = 0; i < n; ++i) {

        // If a box is its own parent, it is the root master of its circuit
        if (dsu.parent[i] == i) {
            circuitSizes.push_back(dsu.size[i]);
        }
    }

    // Sort sizes in descending order (largest first)
    std::sort(circuitSizes.rbegin(), circuitSizes.rend());

    // 5. Multiply the 3 largest circuit sizes together
    long long solution = 1;
    int numMult = std::min(3, (int)circuitSizes.size());
    for (int i = 0; i < numMult; ++i) 
        solution *= circuitSizes[i];
    
    return solution;
}

// =============== PART 2 ===============

long long Playground::solvePart2() {

    int n = junctionBoxes.size();

    // guard 
    if (n < 2) {
        throw std::runtime_error("Error: Not enough coordinates.");
    }

    // initialize the Disjoint Set structure
    DisjointSet dsu(n);
    int successfulMerges = 0;
    long long solution = 0;

    // 4. Continue connecting until only 1 single circuit remains
    for (size_t k = 0; k < this->allEdges.size(); ++k) {

        // if merge operation successful, increment counter
        if (dsu.mergePart2(this->allEdges[k].a, this->allEdges[k].b)) {

            successfulMerges++;

            // we stop after n-1 successful merges
            if (successfulMerges == n - 1) {

                // lookup the two original junction boxes using index trackers
                long long x1 = junctionBoxes[this->allEdges[k].a].x;
                long long x2 = junctionBoxes[this->allEdges[k].b].x;

                solution = x1 * x2;
                // stop loop, since we found last Edge
                break;
            }
        }
    }

    return solution;
}