#include <iostream>
#include "parser.hpp"
#include "plotter.hpp"
#include "viewer.hpp"

int main() {
    // parse_function("x^3 - 2x^2 + 3x - 12");
    std::vector<Point> points = getPoints("x+2", 0, 2);
    for(auto p : points)
        std::cout << "(" << p.x << ", " << p.y << "), ";
}