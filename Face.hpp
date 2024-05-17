//
// Created by maxim on 17.05.2024.
//

#ifndef SIWIR2_EX2_FACE_HPP
#define SIWIR2_EX2_FACE_HPP

#include "Vertex.hpp"

class Face {
public:
    Vertex *vertex0_, *vertex1_, *vertex2_;
    double *A_, *M_;

    Face() = default;
};
#endif //SIWIR2_EX2_FACE_HPP
