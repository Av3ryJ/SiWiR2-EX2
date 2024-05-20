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

void rbgs(double *values, double *func, double alpha, double beta, double gamma, int ny, int nx, int rowlength) {
    for (int iteration = 0; iteration < 100; ++iteration) {
        for (int row = 1; row < ny; ++row) { // row is y col is x
            for (int col = 1; col < nx; ++col) {    // nicht ueber Rand iterieren
                if ((row + col) % 2 == 0) {
                    // RED
                    //std::cout << "RED" << std::endl;
                    values[col + row * rowlength] = (1 / alpha) *
                                                    (func[col + row * rowlength]
                                                     + gamma * values[(col - 1) + row * rowlength]
                                                     + gamma * values[(col + 1) + row * rowlength]
                                                     + beta * values[col + (row - 1) * rowlength]
                                                     + beta * values[col + (row + 1) * rowlength]);
                }
            }
        }
        for (int row = 1; row < ny; ++row) { // row is y col is x
            for (int col = 1; col < nx; ++col) {    // nicht ueber Rand iterieren
                if ((row + col) % 2 == 1) {
                    // BLACK
                    //std::cout << "BLACK" << std::endl;
                    values[col + row * rowlength] = (1 / alpha) *
                                                    (func[col + row * rowlength]
                                                     + gamma * values[(col - 1) + row * rowlength]
                                                     + gamma * values[(col + 1) + row * rowlength]
                                                     + beta * values[col + (row - 1) * rowlength]
                                                     + beta * values[col + (row + 1) * rowlength]);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    //Usage
    if (argc < 3 || argc > 4) {
        cout << "Usage: ./waveguide <delta> <epsilon> [<refLvl>]" << endl;
        return -1;
    }
    //parse args
    delta = atof(argv[1]);
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
    std::ofstream fileKSQ("ksq.txt");

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

    std::ofstream fileA("A.txt");
    std::ofstream fileM("M.txt");

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

    double ew_old;
    double ew_new;

    while(std::abs((ew_new-ew_old)/ew_old)>std::pow(10,-10)){
        ew_old= ew_new;
        //f=Mh*uh


    }




    delete[] vertices;
    delete[] faces;
    delete[] ksq;
    return 0;
}