#include <iomanip>
#include <iostream>
#include <limits>
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

void matrixVectorMultiplication(std::vector<std::map<int, double>> matrix, double *vector, double *result, int length) {
    for (int row = 0; row < length; row++) {
        for (auto col: matrix[row]) {
            result[row] += col.second * vector[col.first];
        }
    }
}

void solver(double *u, double *f, std::vector<std::map<int, double>> globalStiffness, int length, double epsilon) {
    double residual;
    do {
        residual = 0;
        // Gauss-Seidel
        for (int row = 0; row < length; ++row) {
            double sum = f[row];
            for (auto i: globalStiffness[row] ) {
                if (i.first != row) {
                    sum -= i.second * u[i.first];
                }
            }
            u[row] = sum / globalStiffness[row][row]; // u[i] = 1/aii * (f[i] - ai1*u1 - a2*ui2 ..)
        }

        // residual
        auto *residual_vector = new double[length]{};
        matrixVectorMultiplication(globalStiffness, u, residual_vector, length);
        for (int i = 0; i < length; i++) {
            double temp = f[i] - residual_vector[i];
            residual += temp*temp;
        }
        residual = sqrt(residual / length);
        delete[] residual_vector;
    } while (residual > epsilon);
}

int main(int argc, char* argv[]) {
    //Usage
    if (argc < 3 || argc > 4) {
        cout << "Usage: ./waveguide <delta> <epsilon> [<refLvl>]" << endl;
        return -1;
    }
    // ================== parse args ==================
    delta = atof(argv[1]);
    //setting delta for all Faces
    Face::delta_ = delta;

    int refLvl = 0;
    double epsilon = atof(argv[2]);
    if (argc == 4) {
        refLvl = atoi(argv[3]);
    }

    // ================== read vertices and faces from input file ==================
    //input stream from readfile
    ifstream readfile("./unit_circle.txt");

    string line;
    getline(readfile, line);
    int number_of_vertices = atoi(line.c_str());    // get number of vertices from first line

    //skip second line
    getline(readfile, line);

    // array to store vertices
    std::vector<Vertex> vertices;

    std::ofstream fileKSQ("./ksq.txt");

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
        double ksq = computeKSq(x, y);
        fileKSQ << x << " " << y << " " << ksq << "\n";
        delete[] buf;
    }
    fileKSQ.close();

    getline(readfile, line);
    int number_of_faces = atoi(line.c_str());

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

    // ================== Finished reading in inputs ==================

// this code snipped can be used to print out the net for validation
//    std::ofstream fileV("./Vertices.txt");
//    for (Vertex &ver : vertices) {
//        fileV << ver.index_ << " " << ver.x_ << " " << ver.y_ << std::endl;
//    }
//    fileV.close();
//
//    std::ofstream fileF("./Faces.txt");
//    for (Face &fa : faces) {
//        fileF << fa.vertex0_->index_ << " " << fa.vertex1_->index_ << " " << fa.vertex2_->index_ << " " << std::endl;
//    }
//    fileF.close();

    // ================== refinement ==================
    //refine (this is not pretty, but it works :/)
    for (int refinement = 0; refinement < refLvl; refinement++) {
        std::vector<Vertex> new_vertices = vertices;

        std::vector<std::vector<int>> saved_faces;

        Vertex *midpoint0;
        Vertex *midpoint1;
        Vertex *midpoint2;

        for (Face &face: faces) {
            midpoint0 = face.vertex0_->getMidpoint(face.vertex1_, new_vertices);
            midpoint1 = face.vertex0_->getMidpoint(face.vertex2_, new_vertices);
            midpoint2 = face.vertex1_->getMidpoint(face.vertex2_, new_vertices);
            std::vector<int> temp0 = {face.vertex0_->index_, midpoint0->index_, midpoint1->index_};
            std::vector<int> temp1 = {midpoint0->index_, face.vertex1_->index_, midpoint2->index_};
            std::vector<int> temp2 = {midpoint0->index_, midpoint1->index_, midpoint2->index_};
            std::vector<int> temp3 = {midpoint1->index_, midpoint2->index_, face.vertex2_->index_};
            saved_faces.push_back(temp0);
            saved_faces.push_back(temp1);
            saved_faces.push_back(temp2);
            saved_faces.push_back(temp3);
        }

        //prepare for (possible) next iteration
        vertices = new_vertices;
        new_vertices.clear();
        number_of_vertices = (int) vertices.size();
        for (Vertex ver : vertices) {
            ver.midpoints_.clear();
        }

        //create new faces
        std::vector<Face> new_faces;
        new_faces.reserve(saved_faces.size());
        for (std::vector s_face: saved_faces) {
            new_faces.push_back(*new Face(&vertices[s_face[0]], &vertices[s_face[1]], &vertices[s_face[2]]));
        }
        faces = new_faces;
        new_faces.clear();
        number_of_faces = (int) faces.size();
    }

// this code snipped can be used to print out the refined net for validation
//    std::ofstream fileVR("./Vertices_Refined.txt");
//    for (Vertex &ver : vertices) {
//        fileVR << ver.index_ << " " << ver.x_ << " " << ver.y_ << std::endl;
//    }
//    fileVR.close();
//
//    std::ofstream fileFR("./Faces_refined.txt");
//    for (Face &fa : faces) {
//        fileFR << fa.vertex0_->index_ << " " << fa.vertex1_->index_ << " " << fa.vertex2_->index_ << " " << std::endl;
//    }
//    fileFR.close();


    // ================== Fill global mass and stiffness matrices ==================
    std::map<int, double> leer; //muss man anscheinend mitübergeben 
    std::vector<std::map<int, double>> globalStiffness(number_of_vertices, leer);
    std::vector<std::map<int, double>> globalMass(number_of_vertices, leer);

    for (Face &current : faces) {
        int *indices = new int[3]{current.vertex0_->index_, current.vertex1_->index_, current.vertex2_->index_};
        //  add to global matrices
        for (int x = 0; x <= 2; x++) {
            for (int y = 0; y <= 2; y++) {
                auto stiffValue = globalStiffness[indices[x]][indices[y]];
                globalStiffness[indices[x]][indices[y]] = stiffValue + current.A_[x][y];
                auto massValue = globalMass[indices[x]][indices[y]];
                globalMass[indices[x]][indices[y]] = massValue + current.M_[x][y];
            }
        }
        delete[] indices;
    }

    // Write A.txt and M.txt
    std::ofstream fileA("./A.txt");
    std::ofstream fileM("./M.txt");

    for (int row = 0; row < number_of_vertices; row++) {
        for (auto &stiffIter : globalStiffness[row]) {
            fileA << row << " " << stiffIter.first << " " << stiffIter.second << std::endl;
        }
        for (auto &massIter : globalMass[row]) {
            fileM << row << " " << massIter.first << " " << massIter.second << std::endl;
        }
    }

    fileA.close();
    fileM.close();

    // ================== start inverse power iteration ==================

    // init eigenvalue
    double ew_old = 1., ew_new = 0.;
    // init eigenmode
    auto *u = new double[number_of_vertices];
    for (int i = 0; i < number_of_vertices; i++) {
        u[i] = 1.;
    }
    //Write file for the eigenvalue after each iteration
    std::ofstream fileLambda("./lambda.txt");

    while(std::abs((ew_new-ew_old)/ew_old)>std::pow(10,-10)){
        ew_old= ew_new;
        // to store Mh * uh
        auto *f = new double[number_of_vertices]{};
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

        // compute new eigenvalue
        auto *Au = new double[number_of_vertices]{};
        auto *Mu = new double[number_of_vertices]{};
        matrixVectorMultiplication(globalStiffness, u, Au, number_of_vertices);
        matrixVectorMultiplication(globalMass, u, Mu, number_of_vertices);
        double Au_sum = 0, Mu_sum = 0;
        for (int i = 0; i < number_of_vertices; i++) {
            Au_sum += u[i] * Au[i];
            Mu_sum += u[i] * Mu[i];
        }
        ew_new = Au_sum / Mu_sum;
        //output lambda to lamda.txt
        fileLambda << ew_new << std::endl;

        delete[] f;
        delete[] Au;
        delete[] Mu;
    }
    fileLambda.close();

    // Write eigenmode.txt
    std::ofstream fileEigenmode("./eigenmode.txt");

    for (int i = 0; i < number_of_vertices; i++) {
        fileEigenmode << vertices[i].x_ << " " << vertices[i].y_ << " " << u[i] << std::endl;
    }

    fileEigenmode.close();
    

    // pickup trash
    vertices.clear();
    faces.clear();
    globalStiffness.clear();
    globalMass.clear();
    delete[] u;
    return 0;
}