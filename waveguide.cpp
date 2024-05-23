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
            u[row] = sum / matrix[row_index + row]; // u[i] = f[i] - a1*u1 - a2*u2 ...
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

    double epsilon = atof(argv[2]);
    if (argc == 4) {
        int refLvl = atoi(argv[3]);
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
    auto *vertices = new Vertex[number_of_vertices];

    auto *ksq = new double[number_of_vertices];
    std::ofstream fileKSQ("../ksq.txt");

    char delim[] = " ";
    for (int i = 0; i < number_of_vertices; i++) {
        getline(readfile, line);
        char *buf = new char[line.length()];
        strcpy(buf, line.c_str());

        char *index = strtok(buf, delim);
        double x = atof(strtok(NULL, delim));
        double y = atof(strtok(NULL, delim));
        vertices[i].x_ = x;
        vertices[i].y_ = y;
        vertices[i].index_ = atoi(index);
        ksq[i] = computeKSq(x, y);
        fileKSQ << x << " " << y << " " << ksq[i] << "\n";
        delete[] buf;
    }
    fileKSQ.close();

    getline(readfile, line);
    int number_of_faces = atoi(line.c_str());
    cout << number_of_faces << " faces" << endl;

    //array to store face vertices
    Face *faces = new Face[number_of_faces];

    //skip next line
    getline(readfile, line);

    for (int i = 0; i < number_of_faces; i++) {
        getline(readfile, line);
        char *buf = new char[line.length()];
        strcpy(buf, line.c_str());

        int index0 = atoi(strtok(buf, delim));
        int index1 = atoi(strtok(NULL, delim));
        int index2 = atoi(strtok(NULL, delim));
        faces[i].vertex0_ = &vertices[index0];
        faces[i].vertex1_ = &vertices[index1];
        faces[i].vertex2_ = &vertices[index2];
        faces[i].calculateStiffness();
        faces[i].calculateMass();
        delete[] buf;
    }

    //close unit_circle.txt
    readfile.close();

    auto *globalStiffness = new double[number_of_vertices*number_of_vertices];
    auto *globalMass = new double[number_of_vertices*number_of_vertices];

    for (int i = 0; i < number_of_faces; i++) {
        Face *current = &faces[i];
        // add local matrices to global
        //  get indices for vertices
        int *indices = new int[3]{current->vertex0_->index_, current->vertex1_->index_, current->vertex2_->index_};
        //  add to global matrices
        for (int x = 0; x <= 2; x++) {
            for (int y = 0; y <= 2; y++) {
                globalStiffness[indices[x]+indices[y]*number_of_vertices] += current->A_[x][y];
                globalMass[indices[x]+indices[y]*number_of_vertices] += current->M_[x][y];
            }
        }
    }

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

    delete[] vertices;
    delete[] faces;
    delete[] ksq;
    delete[] u;
    delete[] f;
    return 0;
}