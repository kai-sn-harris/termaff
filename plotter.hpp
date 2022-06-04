#pragma once

#include <vector>
#include "parser.hpp"

struct Point {
    int x, y;
};

struct Function {
    // f(x) = 2x + 4

};

std::vector<Point> getPoints(std::string function, int x_min, int x_max);