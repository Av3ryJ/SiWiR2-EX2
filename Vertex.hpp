#include <cmath>
#include <iostream>

#ifndef SIWIR2_EX2_VERTEX_HPP
#define SIWIR2_EX2_VERTEX_HPP

class Vertex {
public:
    double x_, y_, ksq_;
    int index_;

    Vertex() = default;
    Vertex(double x, double y, int index);
    void printVertex();
};

#endif //SIWIR2_EX2_VERTEX_HPP
