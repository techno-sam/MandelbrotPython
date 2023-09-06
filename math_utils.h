#include <functional>
#include <math.h>
#ifndef MATH_UTILS_H_INCLUDED
#define MATH_UTILS_H_INCLUDED

static double absolute(double x, double y) {
    return x*x + y*y;
}

struct Zoom {
    double min_x {};
    double min_y {};
    double max_x {};
    double max_y {};
    unsigned int width {};
    unsigned int height {};

};

//responsibility of caller to destroy pointer
std::vector<std::pair<double, double>>* calculatePath(double x, double y, unsigned int escape);

//samples all points -2 to 2 with 'resolution' points per positive/negative (for total of 2*'resolution' points)
//responsibility of caller to destroy pointer
std::vector<std::vector<unsigned int>>* calculatePaths(Zoom zoom, unsigned int resolution, unsigned int escape, unsigned int *max_val);

void reverseMap(double x, double y, Zoom zoom, unsigned int *x_out, unsigned int *y_out);

#endif // MATH_UTILS_H_INCLUDED
