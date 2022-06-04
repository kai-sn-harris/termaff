#include "plotter.hpp"

std::vector<Point> getPoints(std::string function, int x_min, int x_max) {
    std::vector<Point> points;
    // go over domain
    for(int i = x_min; i < x_max+1; i++) {
        Point point;
        point.x = i;
        point.y = calc(function, i);
        points.push_back(point);
    }
    return points;
}