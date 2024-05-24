#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "cstring"
#include "Vertex.hpp"
#include "Face.hpp"

using namespace std;

static double delta = 0;

double computeKSq(double x, double y) {
    return (100 + delta)*exp(-50*(x*x+y*y)) - 100;
}

void matrixVectorMultiplication(double *matrix, double *vector, double *result, int length) {
    for (int row = 0; row < length; row++) {
        for (int col = 0; col < length; col++) {
            result[row] += matrix[row * length + col] * vector[col];
        }
    }
}

void printVector(double *vec, int len) {
    for (int i = 0; i < len; i++) {
        cout << vec[i] << "  ;  ";
    }
    cout << endl << endl;
}

void solver(double *u, double *f, double *matrix, int length, double epsilon) {
    double residual = 0;
    auto *residual_vector = new double[length];
    int max_iter = 100, counter = 0;
    do {
        for (int row = 0; row < length; ++row) { // row is y col is x
            double sum = f[row];
            int row_index = row * length;
            for (int col = 0; col < length; ++col) {
                if (col != row) {
                    sum -= matrix[row_index + col] * u[col];
                }
            }
            u[row] = sum / matrix[row_index + row]; // u[i] = 1/aii * (f[i] - ai1*u1 - a2*ui2 ...)
        }
        matrixVectorMultiplication(matrix, u, residual_vector, length);
        for (int i = 0; i < length; i++) {
            double temp = f[i] - residual_vector[i];
            residual += temp*temp;
        }
        residual = sqrt(residual / length);

    } while (residual > epsilon && ++counter <= max_iter);

    delete[] residual_vector;
}

int main(int argc, char* argv[]) {
    //Usage
    if (argc < 3 || argc > 4) {
        cout << "Usage: ./waveguide <delta> <epsilon> [<refLvl>]" << endl;
        return -1;
    }
    //parse args
    delta = atof(argv[1]);
    //setting delta for all Faces
    Face::delta_ = delta;

    int refLvl = 0;
    double epsilon = atof(argv[2]);
    if (argc == 4) {
        refLvl = atoi(argv[3]);
    }

    //input stream from readfile
    ifstream readfile("../unit_circle.txt");

    string line;
    getline(readfile, line);
    int number_of_vertices = atoi(line.c_str());    // get number of vertices from first line
    cout << number_of_vertices << " vertices" << endl;

    //skip second line
    getline(readfile, line);

    // array to store vertices
    std::vector<Vertex> vertices;

    auto *ksq = new double[number_of_vertices];
    std::ofstream fileKSQ("../ksq.txt");

    char delim[] = " ";
    for (int i = 0; i < number_of_vertices; i++) {
        getline(readfile, line);
        char *buf = new char[line.length()];
        strcpy(buf, line.c_str());

        char *index = strtok(buf, delim);
        double x = atof(strtok(nullptr, delim));
        double y = atof(strtok(nullptr, delim));
        int iindex = atoi(index);
        vertices.push_back(*(new Vertex(x, y, iindex)));
        ksq[i] = computeKSq(x, y);
        fileKSQ << x << " " << y << " " << ksq[i] << "\n";
        delete[] buf;
    }
    fileKSQ.close();

    getline(readfile, line);
    int number_of_faces = atoi(line.c_str());
    cout << number_of_faces << " faces" << endl;

    //array to store face vertices
    std::vector<Face> faces;

    //skip next line
    getline(readfile, line);

    for (int i = 0; i < number_of_faces; i++) {
        getline(readfile, line);
        char *buf = new char[line.length()];
        strcpy(buf, line.c_str());

        int index0 = atoi(strtok(buf, delim));
        int index1 = atoi(strtok(NULL, delim));
        int index2 = atoi(strtok(NULL, delim));
        faces.push_back(*(new Face(&vertices[index0],
                                   &vertices[index1],
                                   &vertices[index2])));
        delete[] buf;
    }

    //close unit_circle.txt
    readfile.close();

    //refine
    std::cout << "starting to refine" << std::endl;

    for (int refinement = 0; refinement < refLvl; refinement++) {
        std::cout << "refining level: " << refinement+1 << std::endl;
        std::vector<Vertex> new_vertices = vertices;

        std::vector<std::vector<int>> saved_faces;

        Vertex *midpoint0;
        Vertex *midpoint1;
        Vertex *midpoint2;
        std::cout << "iterating faces: " << faces.size() << std::endl;
        int b = 0;
        for (Face &face: faces) {
            //if (b > 7000) std::cout << b << std::endl;
            b++;
            midpoint0 = face.vertex0_->getMidpoint(face.vertex1_, new_vertices);
            midpoint1 = face.vertex0_->getMidpoint(face.vertex2_, new_vertices);
            midpoint2 = face.vertex1_->getMidpoint(face.vertex2_, new_vertices);
            //std::cout << midpoint0->index_ << " " << midpoint1->index_ << " " << midpoint2->index_ << std::endl;
            std::vector<int> temp0 = {face.vertex0_->index_, midpoint0->index_, midpoint1->index_};
            std::vector<int> temp1 = {midpoint0->index_, face.vertex1_->index_, midpoint2->index_};
            std::vector<int> temp2 = {midpoint0->index_, midpoint1->index_, midpoint2->index_};
            std::vector<int> temp3 = {midpoint1->index_, midpoint2->index_, face.vertex2_->index_};
            saved_faces.push_back(temp0);
            saved_faces.push_back(temp1);
            saved_faces.push_back(temp2);
            saved_faces.push_back(temp3);
        }
        std::cout << "faces iterated over" << std::endl;
        vertices = new_vertices;
        number_of_vertices = (int) vertices.size();
        for (Vertex ver : vertices) {
            ver.midpoints_.clear();
        }
        std::cout << "number of faces to create after " << refinement << " steps: " << saved_faces.size() << std::endl;
        std::vector<Face> new_faces;
        new_faces.reserve(saved_faces.size());
        for (std::vector s_face: saved_faces) {
            std::cout << "index0 " << s_face[0] << " index1 " << s_face[1] << " index2 " << s_face[2] << std::endl;
            new_faces.push_back(*new Face(&vertices[s_face[0]], &vertices[s_face[1]], &vertices[s_face[2]]));
        }
        faces = new_faces;
        number_of_faces = (int) faces.size();
    }

    std::cout << "starting to populate global mats..." << std::endl;

    auto *globalStiffness = new double[number_of_vertices*number_of_vertices];
    auto *globalMass = new double[number_of_vertices*number_of_vertices];

    std::cout << "faces len = " << faces.size() << std::endl;
    std::cout << "vertices len = " << vertices.size() << std::endl;

    int n = 0;
    for (Face &current : faces) {
        n++;
        std::cout << n << std::endl;
        // add local matrices to global
        // get indices for vertices
        int *indices = new int[3]{current.vertex0_->index_, current.vertex1_->index_, current.vertex2_->index_};
        std::cout << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
        //  add to global matrices
        for (int x = 0; x <= 2; x++) {
            for (int y = 0; y <= 2; y++) {
                globalStiffness[indices[x]+indices[y]*number_of_vertices] += current.A_[x][y];
                globalMass[indices[x]+indices[y]*number_of_vertices] += current.M_[x][y];
            }
        }
        delete[] indices;
    }

    std::cout << "Finished to polutlate global mats. Writing to files..." << std::endl;

    std::ofstream fileA("../A.txt");
    std::ofstream fileM("../M.txt");

    for (int x = 0; x < number_of_vertices; x++) {
        for (int y = 0; y < number_of_vertices; y++) {
            if (globalStiffness[x + y * number_of_vertices] != 0) {
                fileA << x << " " << y << " " << globalStiffness[x + y * number_of_vertices] << std::endl;
            }
            if (globalMass[x + y * number_of_vertices] != 0) {
                fileM << x << " " << y << " " << globalMass[x + y * number_of_vertices] << std::endl;
            }
        }
    }

    fileA.close();
    fileM.close();

    std::cout << "finished writing A.txt and M.txt. Starting eigenmode calculation..." << std::endl;

    // eigenvalue
    double ew_old = 1., ew_new = 0.;
    // init eigenmode
    auto *u = new double[number_of_vertices];
    for (int i = 0; i < number_of_vertices; i++) {
        u[i] = 0.5;
    }
    // to store Mh * uh
    auto *f = new double[number_of_vertices];

    int c = 0;

    while(std::abs((ew_new-ew_old)/ew_old)>std::pow(10,-10) && ++c <= 30){
        ew_old= ew_new;
        //f=Mh*uh
        matrixVectorMultiplication(globalMass, u, f, number_of_vertices);
        //solver
        solver(u, f, globalStiffness, number_of_vertices, epsilon);

        // normalize uh
        double norm = 0;
        for (int i = 0; i < number_of_vertices; i++) {
            norm += u[i]*u[i];
        }
        norm = sqrt(norm);
        for (int i = 0; i < number_of_vertices; i++) {
            u[i] = u[i] / norm;
        }
        //printVector(u, 10);

        // compute new eigenvalue
        auto *Au = new double[number_of_vertices];
        auto *Mu = new double[number_of_vertices];
        matrixVectorMultiplication(globalStiffness, u, Au, number_of_vertices);
        matrixVectorMultiplication(globalMass, u, Mu, number_of_vertices);
        double Au_sum = 0, Mu_sum = 0;
        for (int i = 0; i < number_of_vertices; i++) {
            Au_sum += u[i] * Au[i];
            Mu_sum += u[i] * Mu[i];
        }
        ew_new = Au_sum / Mu_sum;

        delete[] Au;
        delete[] Mu;
    }

    std::ofstream fileEigenmode("../eigenmode.txt");

    for (int i = 0; i < number_of_vertices; i++) {
        fileEigenmode << vertices[i].x_ << " " << vertices[i].y_ << " " << u[i] << std::endl;
    }

    fileEigenmode.close();

    for (int i = 0; i < number_of_vertices; i++) {
        delete &vertices[i];
    }
    for (int i = 0; i < number_of_faces; i++) {
        delete &faces[i];
    }

    vertices.clear();
    faces.clear();
    delete[] globalStiffness;
    delete[] globalMass;
    delete[] ksq;
    delete[] u;
    delete[] f;
    return 0;
}