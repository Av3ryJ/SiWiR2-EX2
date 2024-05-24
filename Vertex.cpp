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
    int c = (int) this->midpoints_.count(otherVertex->index_);
    if (c > 0) {
        return &vertices[this->midpoints_.find(otherVertex->index_)->first];
    }
    else {
        int new_index =  (int) vertices.size();

        this->midpoints_.insert({otherVertex->index_, new_index});
        otherVertex->midpoints_.insert({this->index_, new_index});

        vertices.push_back(*(new Vertex(((otherVertex->x_ - this->x_)/2) + this->x_,
                                      ((otherVertex->y_ - this->y_)/2) + this->y_,
                                      new_index)));

        return &vertices[new_index];
    }
}

