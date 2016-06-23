cmake_minimum_required(VERSION 3.5)
project(OsmToGunrock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Osmium REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(OsmToGunrock ${SOURCE_FILES})