Polyhex Sampler
===============
[![Build Status](https://travis-ci.org/polyhex-sampling/sampler.svg?branch=master)](https://travis-ci.org/polyhex-sampling/sampler)


Description
-----------

Build
-----

To build the sampler, several packages are requires:

* cmake (http://cmake.org) to generate the build project.
* Qt4 (Gui) (http://qt.digia.com) 
* Boost (and boost-progam-options) for command-line parsing (http://boost.org)
* OpenGL
* SDL and SDL_image


A classical commandline build process looks like

    mkdir build  ##Generate build folder
    cd build
    cmake ..     ##Check dependencies and generate a makefile
    make         ##Build

For best performances, you can compile with optimized flags: `cmake .. -DCMAKE_BUILD_TYPE=Release`

Once built, several executables are available in `build/bin`:

* `sampler-cli`: command line uniform sampling tool
* `sampler-gui`: Qt base sampler (uniform, adaptive, ...) (needs openGL>3.3)
* `sampler-test`: to check integrity of data files


Data
----

This program needs full offsets table to produce best sampling distribution.
These tables are heavy (1.5G) and can be downloaded here: http://liris.cnrs.fr/polyhex/data/

Aternatively, `make GetLUT` (GetLUT target in your IDE) will download the data file to the folder `{your-build-folder}/data/lut`.


Usage examples
--------------



LICENSE
-------

The MIT License (MIT)

Copyright (c) 2014 Polyhex sampling

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
