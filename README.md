CS-Image-Segmentation-Solver
============================

Project 1 for 3460:635 Advanced Algorithms.

### Project Description

The objective of this project is to implement the Ford-Fulkerson network flow algorithm and then use
it to solve an image segmentation problem. The image, stored in a plain pgm file, is split into two
parts (foreground and background) using the algorithm.

See the <a href="http://www.cs.uakron.edu/~zduan/class/635/projects/project1/project1.htm">program
instructions</a> for more details.

### Compilation
Enter project root directory and run:
`make`

Binaries are located in bin directory.

### CLI Client Command Usage:
Breadth First Search
`./bin/iseg -b input file start vertex end vertex`

Ford-Fulkerson
`./bin/iseg -f input file`

Image Segmentation
`./bin/iseg -i input file ouput file`

### Test Suite:
Full test suite `./bin/test-suite`

### Test Cases:
Text Based Graphs
test/graphs/*

PGMs
test/pgm/*

### Developed and Tested on OSX Mavericks 64-bit