#include "Face.hpp"

 double Face::computeKSq(double x, double y) {
    return (100 + delta_)*exp(-50*(x*x+y*y)) - 100;
}

void Face::calculateStiffness() {
    ELEMENTS::Triangle my_element;

    std::vector<double> verts(6, 0.0);

    verts[0] = vertex0_->x_; verts[1] = vertex0_->y_;
    verts[2] = vertex1_->x_; verts[3] = vertex1_->y_;
    verts[4] = vertex2_->x_; verts[5] = vertex2_->y_;
    my_element(verts);
    // for local stiffness
    A_ = my_element.integrate(grad(v_())*grad(w_()) - func<double>(computeKSq)*v_()*w_());
}

void Face::calculateMass() {
    //TODO

    // for local mass matrix
    //M_ = my_element.integrate(v_()*w_());
    // add entries from my_local_matrix to global mass matrix
    // ...
}