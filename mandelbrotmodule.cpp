#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <complex>
#include <iostream>
#include <functional>
#include <array>
#include "math_utils.h"

// args and return (python style) (scaled_min_x: float, scaled_min_y: float,
//                                 scaled_max_x: float, scaled_max_y: float,
//                                 width: int, height: int, escape: int, resolution: int)
//                                 -> (max_val, tuple<x>[tuple<y>[int<density>]])
static PyObject *
mandelbrot_paths(PyObject *self, PyObject *args)
{
    double smin_x;
    double smin_y;
    double smax_x;
    double smax_y;
    unsigned int width;
    unsigned int height;
    unsigned int escape;
    unsigned int resolution;

    if (!PyArg_ParseTuple(args, "ddddIIII",
                          &smin_x, &smin_y, &smax_x, &smax_y,
                          &width, &height, &escape, &resolution))
        return NULL;

    unsigned int max_val = 0;
    std::vector<std::vector<unsigned int>> *paths
          = calculatePaths(Zoom{smin_x, smin_y, smax_x, smax_y, width, height},
                           resolution, escape, &max_val);
    PyObject *out = PyTuple_New(2);
    PyObject *tupleX = PyTuple_New(width);
    for (unsigned int x = 0; x < width; x++) {
        PyObject *tupleY = PyTuple_New(height);
        for (unsigned int y = 0; y < height; y++) {
            PyTuple_SET_ITEM(tupleY, y, PyLong_FromLong((*paths)[x][y]));
        }
        PyTuple_SET_ITEM(tupleX, x, tupleY);
    }
    delete paths;
    PyTuple_SET_ITEM(out, 0, PyLong_FromLong(max_val));
    PyTuple_SET_ITEM(out, 1, tupleX);
    return out;
}


static PyObject *
mandelbrot_path(PyObject *self, PyObject *args)
{
    double x;
    double y;
    unsigned int escape;

    if (!PyArg_ParseTuple(args, "ddI", &x, &y, &escape))
        return NULL;

    std::vector<std::pair<double, double>> points = std::vector<std::pair<double, double>>();

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

        points.push_back(std::pair(curr_x, curr_y));

        if (absolute(curr_x, curr_y) > 4) {
            PyObject *tp = PyTuple_New(points.size());
            for (unsigned int i = 0; i < points.size(); i++) {
                PyObject *sub = PyTuple_New(2);
                PyTuple_SET_ITEM(sub, 0, PyFloat_FromDouble(points[i].first));
                PyTuple_SET_ITEM(sub, 1, PyFloat_FromDouble(points[i].second));
                PyTuple_SET_ITEM(tp, i, sub);
            }
            return tp;
            //return PyLong_FromLong(iters);
        }
    }
    return PyTuple_New(0);//PyLong_FromLong(-1);
}


static PyObject *
mandelbrot_point(PyObject *self, PyObject *args)
{
    double x;
    double y;
    unsigned int escape;

    if (!PyArg_ParseTuple(args, "ddI", &x, &y, &escape))
        return NULL;

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

        if (absolute(curr_x, curr_y) > 4)
            return PyLong_FromLong(iters);
    }
    return PyLong_FromLong(-1);
}

/*int run_escape(std::complex<double> c, int iter_max, const std::function<std::complex<double>(std::complex<double>,
 	std::complex<double>)> &func) {
 	std::complex<double> z(0);
 	int iter = 0;

 	while (abs(z) < 2.0 && iter < iter_max) {
 		z = func(z, c);
 		iter++;
 	}

 	return iter;
}

static PyObject *
mandelbrot_point(PyObject *self, PyObject *args)
{
    double x;
    double y;
    unsigned int escape;

    if (!PyArg_ParseTuple(args, "ddI", &x, &y, &escape))
        return NULL;

    auto func = [] (std::complex <double> z, std::complex<double> c) -> std::complex<double> {return z * z + c; };

    std::complex<double> c((double)x, (double)y);
    int esc = run_escape(c, escape, func);
    if (esc == escape)
        return PyLong_FromLong(-1);
    return PyLong_FromLong(esc);
}*/

static PyMethodDef MandelbrotMethods[] = {
    {"point",  mandelbrot_point, METH_VARARGS,
     "Calculate escape time for a point."},
    {"path", mandelbrot_path, METH_VARARGS, "Calculate path traveled for a point"},
    {"paths", mandelbrot_paths, METH_VARARGS, "Calculate paths"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef mandelbrotmodule = {
    PyModuleDef_HEAD_INIT,
    "mandelbrot",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    MandelbrotMethods
};

PyMODINIT_FUNC
PyInit_mandelbrot(void)
{
    return PyModule_Create(&mandelbrotmodule);
}



