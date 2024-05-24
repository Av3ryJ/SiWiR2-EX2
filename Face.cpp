#include "Face.hpp"
#include "Colsamm/Colsamm.h"

double Face::delta_;

Face::Face(Vertex *vertex0, Vertex *vertex1, Vertex *vertex2) {
    vertex0_ = vertex0;
    vertex1_ = vertex1;
    vertex2_ = vertex2;

    calculateStiffness();
    calculateMass();
}

double Face::computeKSq(double x, double y) {
    return (100 + delta_)*exp(-50*(x*x+y*y)) - 100;
}

void Face::calculateStiffness() {
    using namespace _COLSAMM_;
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
    using namespace _COLSAMM_;
    ELEMENTS::Triangle my_element;

    std::vector<double> verts(6, 0.0);

    verts[0] = vertex0_->x_; verts[1] = vertex0_->y_;
    verts[2] = vertex1_->x_; verts[3] = vertex1_->y_;
    verts[4] = vertex2_->x_; verts[5] = vertex2_->y_;
    my_element(verts);
    // for local stiffness
    M_ = my_element.integrate(v_()*w_());
}
/*
 * This function will split this Face into 4 smaller ones by taking the midpoints of the 3 edges as new Vertices
 * and constructing 4 smaller Faces
 */
void Face::refine(std::vector<Face> &faces, std::vector<Vertex> &vertices, int own_index) {

}
