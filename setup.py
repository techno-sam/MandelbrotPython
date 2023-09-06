from distutils.core import setup, Extension

module1 = Extension('mandelbrot',
                    sources = ['mandelbrotmodule.cpp', 'math_utils.cpp'])

setup (name = 'Mandelbrot',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
