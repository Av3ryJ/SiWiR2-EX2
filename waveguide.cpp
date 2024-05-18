#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "cstring"
#include "Colsamm/Colsamm.h"
#include "Vertex.hpp"
#include "Face.hpp"

using namespace std;
using namespace ::_COLSAMM_;

double delta = 0;

double computeKSq(double x, double y) {
    return (100 + delta)*exp(-50*(x*x+y*y)) - 100;
}



int main(int argc, char* argv[]) {
    //Usage
    if (argc < 3 || argc > 4) {
        cout << "Usage: ./waveguide <delta> <epsilon> [<refLvl>]" << endl;
        return -1;
    }
    //parse args
    delta = atof(argv[1]);
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
        char buf[line.length()];
        strcpy(buf, line.c_str());

        char *index = strtok(buf, delim);
        double x = atof(strtok(NULL, delim));
        double y = atof(strtok(NULL, delim));
        vertices[i].x_ = x;
        vertices[i].y_ = y;
        vertices[i].index_ = atoi(index);
        ksq[i] = computeKSq(x, y);
        fileKSQ << x << " " << y << " " << ksq[i] << "\n";
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
        char buf[line.length()];
        strcpy(buf, line.c_str());

        int index0 = atoi(strtok(buf, delim));
        int index1 = atoi(strtok(NULL, delim));
        int index2 = atoi(strtok(NULL, delim));
        faces[i].vertex0_ = &vertices[index0];
        faces[i].vertex1_ = &vertices[index1];
        faces[i].vertex2_ = &vertices[index2];
        faces[i].delta_ = delta;
        faces[i].calculateStiffness();
        faces[i].calculateMass();
    }

    //close unit_circle.txt
    readfile.close();

    for (int i = 0; i < number_of_faces; i++) {
        Face triangle = faces[i];
        // add local matrices to global
        // ...
    }


    delete[] vertices;
    delete[] faces;
    delete[] ksq;
    return 0;
}