
# OSM to Matrix Market converter

This is a C++ program that uses libosmium to read OSM data. Stored routable paths using BGL. writes out  

## Prerequisites
Set up [Libosmium](http://osmcode.org/libosmium).
Also the Boost library (for Boost graphs)

## Building

Open the project in CLion, or:


    mkdir build
    cd build
    cmake ..
    make


## TODO
* Better MTX output(header)
* Tests to see if one way logic is working as expected
* Optimizations, if needed to run large OSM's such as the planet.
