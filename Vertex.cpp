#include "Vertex.hpp"

Vertex::Vertex(double x, double y, int index) {
    x_ = x;
    y_ = y;
    index_ = index;
}

// calculates and creates midpoint between self and other vertex
Vertex* Vertex::getMidpoint(Vertex *otherVertex, std::vector<Vertex> &vertices) {
    // first check if midpoint exists already
    int c = (int) this->midpoints_.count(otherVertex->index_);
    if (c > 0) {
        // if it exists just return it
        return &vertices[this->midpoints_.find(otherVertex->index_)->second];
    }
    else {
        // else ceate it with index:
        int new_index =  (int) vertices.size();
        // remeber and tell the other vertex about the new one
        this->midpoints_.insert({otherVertex->index_, new_index});
        otherVertex->midpoints_.insert({this->index_, new_index});
        // create the new vertex and insert it into global list of vertices
        vertices.push_back(*(new Vertex(((otherVertex->x_ - this->x_)/2.) + this->x_,
                                      ((otherVertex->y_ - this->y_)/2.) + this->y_,
                                      new_index)));
        // return new vertex
        return &vertices[new_index];
    }
}

