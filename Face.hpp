#ifndef SIWIR2_EX2_FACE_HPP
#define SIWIR2_EX2_FACE_HPP

#include "Vertex.hpp"

class Face {
public:
    Vertex *vertex0_, *vertex1_, *vertex2_;
    std::vector<std::vector<double>> A_, M_;
    static double delta_;

    Face(Vertex *vertex0, Vertex *vertex1, Vertex *vertex2);
    Face(int index0, int index1, int index2, std::vector<Vertex> vertices);

    static double computeKSq(double x, double y);
    void calculateStiffness();
    void calculateMass();
    void refine(std::vector<Face> &faces, std::vector<Vertex> &vertices, int own_index);
};
#endif //SIWIR2_EX2_FACE_HPP
