#include <iostream>
#include <fstream>
#include <string>
#include "cstring"

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

    file.close();

    //...


    delete[] vertices;
    return 0;
}