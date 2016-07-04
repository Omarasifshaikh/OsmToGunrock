
# OSM to Matrix Market converter

This is a C++ program that uses libosmium to read OSM data; Stores routable paths in memory using BGL. Writes out an MTX that is compatible with Gunrock.

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
* Tests to see if one way logic(one-way roads) is working as expected
* Optimizations, if needed to run large OSM's such as the planet.osm.pbf
