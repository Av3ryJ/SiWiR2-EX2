#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#ifndef SIWIR2_EX2_VERTEX_HPP
#define SIWIR2_EX2_VERTEX_HPP

class Vertex {
public:
    double x_, y_;
    int index_;
    std::map<int, int> midpoints_;

    Vertex(double x, double y, int index);
    Vertex* getMidpoint(Vertex *otherVertex, std::vector<Vertex> &vertices);
};

#endif //SIWIR2_EX2_VERTEX_HPP
