#include "Vertex.hpp"

void Vertex::printVertex() {
    std::cout << index_ << " " << x_ << " " << y_ << std::endl;
}

Vertex::Vertex(double x, double y, int index) {
    x_ = x;
    y_ = y;
    index_ = index;
}

