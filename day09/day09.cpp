#include "day09.h"

#include <fstream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <iostream>

// =============== PUZZLE INPUT ===============

void MovieTheater::readPuzzleInput() {

    // clear previous results
    points.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    long long x;
    long long y;
    char comma;

    while (file >> x >> comma >> y) {

        // validity check as guard 
        if (comma == ',') {
            Point p;
            p.x = x;
            p.y = y;
            points.push_back(p);
        }
    }

    file.close();
}


// =============== PART 1 ===============

long long MovieTheater::solvePart1() {

    size_t n = points.size();

    if (n < 1) return 0;

    long long maxArea = 0;
    
    // scan all possible pairs
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = i + 1; j < n; ++j) {

            long long width = (std::abs(points[i].x - points[j].x) + 1);
            long long height = (std::abs(points[i].y - points[j].y) + 1);
            long long currentArea = width * height;
            
            // check if larger found
            if (currentArea > maxArea)
                maxArea = currentArea;
        }
    }

    return maxArea;
}

// =============== PART 2 ===============

bool MovieTheater::isValidRectangle(const Point& p1, const Point& p2, const std::vector<LineSegment>& edges) {

    long long min_x = std::min(p1.x, p2.x);
    long long max_x = std::max(p1.x, p2.x);
    long long min_y = std::min(p1.y, p2.y);
    long long max_y = std::max(p1.y, p2.y); 

    // 1. Edge Intersection Check
    for (const auto& edge : edges) {

        // check if horizontal edge
        if (edge.y1 == edge.y2) {
            long long edge_y = edge.y1;
            long long edge_min_x = std::min(edge.x1, edge.x2);
            long long edge_max_x = std::max(edge.x1, edge.x2);

            // check if it cuts strictly through the interior Y of the rectangle
            if (edge_y > min_y && edge_y < max_y) {
                // check if it overlaps with the interior X of the rectangle
                if (edge_min_x < max_x && edge_max_x > min_x) {
                    return false;
                }
            }
        }

        // check if vertical edge
        else if (edge.x1 == edge.x2) {
            long long edge_x = edge.x1;
            long long edge_min_y = std::min(edge.y1, edge.y2);
            long long edge_max_y = std::max(edge.y1, edge.y2);

            // check if it cuts strictly throught the interior X of the rectangle
            if (edge_x > min_x && edge_x < max_x) {
                // check if it overlaps with the interior Y of the rectangle
                if (edge_min_y < max_y && edge_max_y > min_y) {
                    return false;
                }
            }
        }
    }

    // 2. Point-in-Polygon (Ray Casting)
    double mid_x = (p1.x + p2.x) / 2.0;
    double mid_y = (p1.y + p2.y) / 2.0;

    // offset slightly to avoid exact vertices collisions in next cast
    double ray_y = mid_y + 0.1;

    int intersections = 0;
    for (const auto& edge : edges) {

        // check that a "casted" horizontal ray only intersects with vertical edges
        if (edge.x1 == edge.x2) {
            long long edge_x = edge.x1;
            double edge_min_y = std::min(edge.y1, edge.y2);
            double edge_max_y = std::max(edge.y1, edge.y2);

            // if edge is to the right of our center, and crosses the Y path in our "casted" ray
            if (edge_x > mid_x && ray_y > edge_min_y && ray_y < edge_max_y) {
                intersections++;
            }
        }
    }

    // we return true if intersections are odd (inside), false if even (outside)
    return (intersections % 2 != 0);
}


long long MovieTheater::solvePart2() {

    size_t n = points.size();

    // guard
    if (n < 4) return 0;

    // extract polygon boundary edges
    std::vector<LineSegment> edges;
    edges.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        Point current = points[i];
        // we use modulo operator to wrap last point back to index 0
        Point next = points[(i + 1) % n];
        edges.push_back({current.x, current.y, next.x, next.y});
    }

    long long maxArea = 0;

    // same brute-force approach as in Part 1
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {

            // only calculate area if rectangle passes validity checks
            if (isValidRectangle(points[i], points[j], edges)) {

                long long width = (std::abs(points[i].x - points[j].x) + 1);
                long long height = (std::abs(points[i].y - points[j].y) + 1);
                long long currentArea = width * height;

                if (currentArea > maxArea) 
                    maxArea = currentArea;
            }
        }
    }

    return maxArea;
}