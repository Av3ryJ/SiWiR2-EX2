#include "Vertex.hpp"

void Vertex::computeKsq(double delta) {
    ksq_ = (100 + delta)*exp(-50*x_*x_+y_*y_) - 100;
}

void Vertex::printVertex() {
    std::cout << index_ << " " << x_ << " " << y_ << std::endl;
}

double Vertex::get_midpoint(double x, double y) {
    return 0; //TODO for extra points
}

