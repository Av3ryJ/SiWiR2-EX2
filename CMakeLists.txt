cmake_minimum_required(VERSION 3.28)
project(waveguide)

set(CMAKE_CXX_STANDARD 14)

add_executable(waveguide
        waveguide.cpp
        Vertex.hpp
        Vertex.cpp
        Face.hpp
        Face.cpp)
