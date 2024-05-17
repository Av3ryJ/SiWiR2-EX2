#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <forward_list>
#include "cstring"

void computeKSquared(double *vertices, double delta, double *ksq, int number_of_vertices) {
    double factor = 100 + delta;
    for (int i = 0; i < number_of_vertices; i++) {
        double x = vertices[2*i];
        double y = vertices[2*i+1];
        ksq[i] = factor*exp(-50*x*x+y*y) - 100;
    }
}

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

    //input stream from file
    ifstream file("../unit_circle.txt");

    string line;
    getline(file, line);
    int number_of_vertices = atoi(line.c_str());    // get number of vertices from first line
    cout << number_of_vertices << endl;

    //skip second line
    getline(file, line);

    // array to store coordinates of vertices: [x0, y0, x1, y1, x2, y2, ...]
    double *vertices = new double[2*number_of_vertices];

    char delim[] = " ";
    for (int i = 0; i < number_of_vertices; i++) {
        getline(file, line);
        char buf[line.length()];
        strcpy(buf, line.c_str());

        char *index = strtok(buf, delim);
        char *x = strtok(NULL, delim);
        char *y = strtok(NULL, delim);
        //cout << "index = " << index << "; x = " << x << "; y = " << y << endl;
        vertices[2*i] = atof(x);
        vertices[2*i+1] = atof(y);
    }

    getline(file, line);
    int number_of_faces = atoi(line.c_str());
    cout << number_of_faces << endl;
    //array to store face vertices: [v1.1, v1.2, v1.3, v2.1, v2.2, v2.3, ...]
    int *faces = new int[3 * number_of_faces];

    map<int, forward_list<int>> neighbors;

    //skip next line
    getline(file, line);

    for (int i = 0; i < number_of_faces; i++) {
        getline(file, line);
        char buf[line.length()];
        strcpy(buf, line.c_str());

        int index0 = atoi(strtok(buf, delim));
        int index1 = atoi(strtok(NULL, delim));
        int index2 = atoi(strtok(NULL, delim));
        faces[3*i] = index0;
        faces[3*i+1] = index1;
        faces[3*i+2] = index2;

        //add indices to neighbors map. catch double added vertices?
        neighbors[index0].push_front(index1);
        neighbors[index0].push_front(index2);
        neighbors[index1].push_front(index0);
        neighbors[index1].push_front(index2);
        neighbors[index2].push_front(index0);
        neighbors[index2].push_front(index1);
    }

    file.close();

    double *ksq = new double[number_of_vertices];
    computeKSquared(vertices, delta, ksq, number_of_vertices);
    //...


    delete[] vertices;
    delete[] faces;
    delete[] ksq;
    return 0;
}