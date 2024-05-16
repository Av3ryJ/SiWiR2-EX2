#include <iostream>
#include <fstream>

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


    //...
}