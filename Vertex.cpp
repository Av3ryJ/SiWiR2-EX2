#include "Vertex.hpp"

void Vertex::printVertex() {
    std::cout << index_ << " " << x_ << " " << y_ << std::endl;
}

Vertex::Vertex(double x, double y, int index) {
    x_ = x;
    y_ = y;
    index_ = index;
}

Vertex* Vertex::getMidpoint(Vertex *otherVertex, std::vector<Vertex> &vertices) {
    std::cout << "getMidpoint in Vertex " << index_ << " with Vertex " << otherVertex->index_ << std::endl;
    int c = this->midpoints_.count(otherVertex->index_);
    if (c > 1) std::cout << "c: " << c << std::endl;
    if (c > 0) {
        std::cout << "FOUND DOUBLE" << std::endl;
        return &vertices[this->midpoints_.find(otherVertex->index_)->first];
    }
    else {
        int new_index =  (int) vertices.size();

        this->midpoints_.emplace(otherVertex->index_, new_index);
        otherVertex->midpoints_.emplace(this->index_, new_index);

        vertices.push_back(*new Vertex(((otherVertex->x_ - this->x_)/2) + this->x_,
                                      ((otherVertex->y_ - this->y_)/2) + this->y_,
                                      new_index));

        return &vertices[new_index];
    }
}

