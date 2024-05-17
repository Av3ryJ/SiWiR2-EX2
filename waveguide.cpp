#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "cstring"
#include "Colsamm/Colsamm.h"
#include "Vertex.hpp"
#include "Face.hpp"

int main(int argc, char* argv[]) {
    using namespace std;
    //Usage
    if (argc < 3 || argc > 4) {
        cout << "Usage: ./waveguide <delta> <epsilon> [<refLvl>]" << endl;
        return -1;
    }
    //parse args
    double delta = atof(argv[1]);
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

    char delim[] = " ";
    for (int i = 0; i < number_of_vertices; i++) {
        getline(readfile, line);
        char buf[line.length()];
        strcpy(buf, line.c_str());

        char *index = strtok(buf, delim);
        char *x = strtok(NULL, delim);
        char *y = strtok(NULL, delim);
        vertices[i].x_ = atof(x);
        vertices[i].y_ = atof(y);
        vertices[i].index_ = atoi(index);
        vertices[i].computeKsq(delta);
    }

    getline(readfile, line);
    int number_of_faces = atoi(line.c_str());
    cout << number_of_faces << " faces" << endl;
    //array to store face vertices: [v1.1, v1.2, v1.3, v2.1, v2.2, v2.3, ...]
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
    }

    //close unit_circle.txt
    readfile.close();

    auto *ksq = new double[number_of_vertices];
    std::ofstream fileKSQ("ksq.txt");
    for (int i = 0; i < number_of_vertices; i++) {
        ksq[i] = vertices[i].ksq_;
        fileKSQ << vertices[i].x_ << " " << vertices[i].y_ << " " << ksq[i] << "\n";
    }
    fileKSQ.close();

    delete[] vertices;
    delete[] faces;
    delete[] ksq;
    return 0;
}