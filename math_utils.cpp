#include "math_utils.h"

std::vector<std::pair<double, double>>* calculatePath(double x, double y, unsigned int escape)
{
    std::vector<std::pair<double, double>>* points = new std::vector<std::pair<double, double>>();
    double curr_x = 0;
    double curr_y = 0;

    double old_x;
    double old_y;
    for (unsigned int iters = 0; iters < escape; iters++) {
        old_x = curr_x;
        old_y = curr_y;

        curr_x = (old_x * old_x) - (old_y * old_y);
        curr_y = 2 * old_x * old_y;

        curr_x += x;
        curr_y += y;

        if (absolute(curr_x, curr_y) >= 4) {
            //points->clear();
            return points;
        }

        points->push_back(std::pair(curr_x, curr_y));
    }
    points->clear();
    return points;
}

std::vector<std::vector<unsigned int>>* calculatePaths(
                    Zoom zoom, unsigned int resolution,
                    unsigned int escape, unsigned int *max_val)
{
    std::vector<std::vector<unsigned int>>* frequency
            = new std::vector<std::vector<unsigned int>>(zoom.width);
    for (unsigned int x = 0; x < zoom.width; x++) {
        //int column[zoom.height];
        //frequency[x] = column;
        /*frequency->push_back(std::vector<unsigned int>(zoom.height));
        for (unsigned int y = 0; y < zoom.height; y++) {
            (*frequency)[x].push_back(0);
        }*/
        (*frequency)[x] = std::vector<unsigned int>(zoom.height);
    }
    double scaleFactor = 2.0/resolution;
    for (int rx = -resolution; rx < (int)resolution; rx++) {
        for (int ry = -resolution; ry < (int)resolution; ry++) {
            double sx = rx*scaleFactor; //in mandelbrot space
            double sy = ry*scaleFactor;
            std::vector<std::pair<double, double>>* points = calculatePath(sx, sy, escape);
            for (long unsigned int i = 0; i < points->size(); i++) {
                unsigned int x; // in screen space
                unsigned int y;
                reverseMap((*points)[i].first, (*points)[i].second, zoom, &x, &y);
                if (0 < x && x < zoom.width && 0 < y && y < zoom.height) {
                    (*frequency)[x][y] += 1;//= frequency[x][y] + 1;
                    /*if ((*frequency)[x][y] > *max_val) {
                        *max_val = (*frequency)[x][y];
                    }*/
                }
            }
            delete points;
        }
    }
    unsigned int mx = 0;
    /*for (int x = 0; x < zoom.width; x++) {
        for (int y = 0; y < zoom.height; y++) {
            if ((*frequency)[x][y] > mx) {
                mx = (*frequency)[x][y];
            }
        }
    }
    *max_val = mx;*/
    for (auto col : *frequency) {
        for (auto f : col) {
            if (f > mx) {
                mx = f;
            }
        }
    }
    *max_val = mx;
    return frequency;
}

void reverseMap(double x, double y, Zoom zoom, unsigned int *x_out, unsigned int *y_out)
{
    double x_range = zoom.max_x - zoom.min_x;
    double y_range = zoom.max_y - zoom.min_y;
    *x_out = (int)((x-zoom.min_x) / x_range * zoom.width);
    *y_out = (int)((y-zoom.min_y) / y_range * zoom.height);
}

