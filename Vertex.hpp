#include <cmath>
#include <iostream>

#ifndef SIWIR2_EX2_VERTEX_HPP
#define SIWIR2_EX2_VERTEX_HPP

class Vertex {
public:
    double x_, y_, ksq_;
    int index_;

    Vertex() = default;
    void computeKsq(double delta);
    void printVertex();
    double get_midpoint(double x, double y);
};

#endif //SIWIR2_EX2_VERTEX_HPP
