cmake_minimum_required(VERSION 2.6)

SET(CMAKE_BUILD_TYPE "RELEASE") 
#include_directories($ENV{EIGEN3_INCLUDE_DIR})

ADD_EXECUTABLE(eigenLib eigenLib.cpp)
ADD_EXECUTABLE(eigenState eigenState.cpp)
ADD_EXECUTABLE(transform transform.cpp)

