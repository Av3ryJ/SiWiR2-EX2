#ifndef SIWIR2_EX2_FACE_HPP
#define SIWIR2_EX2_FACE_HPP

#include "Vertex.hpp"
#include "Colsamm/Colsamm.h"

using namespace _COLSAMM_;

class Face {
public:
    Vertex *vertex0_, *vertex1_, *vertex2_;
    std::vector<std::vector<double>> A_, M_;
    static double delta_;

    Face() = default;

    static double computeKSq(double x, double y);
    void calculateStiffness();
    void calculateMass();
};
#endif //SIWIR2_EX2_FACE_HPP
