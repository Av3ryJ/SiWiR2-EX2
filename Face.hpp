#ifndef SIWIR2_EX2_FACE_HPP
#define SIWIR2_EX2_FACE_HPP

#include "Vertex.hpp"
#include <vector>

class Face {
public:
    Vertex *vertex0_, *vertex1_, *vertex2_;
    std::vector<std::vector<double>> A_, M_;
    static double delta_;

    Face() = default;
    Face(Vertex *vertex0, Vertex *vertex1, Vertex *vertex2);

    static double computeKSq(double x, double y);
    void calculateStiffness();
    void calculateMass();
};
#endif //SIWIR2_EX2_FACE_HPP
