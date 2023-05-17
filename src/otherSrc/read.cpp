#include <iostream>
using std::cout;
using std::cin;

#include <fstream>
using std::ifstream;
using std::ios;


//typedef float real;


#include "dimen.h"    

int main ()
{
    ifstream myFile;
    myFile.open("ade1.dat", ios::in);
    float *w;
    
    dimVec(w,13);
    //myFile.seekg(0,ios::beg);
    myFile.read(reinterpret_cast< char *> (&w[1]), 13*sizeof(float));
    cout << w[1] << "   ";
    cout << w[2] << "   ";
    cout << w[3] << "   ";
    cout << w[4] << "   ";
    cout << w[5] << "   ";
    cout << w[13] << "   \n";

    myFile.read(reinterpret_cast< char *> (&w[1]), 13*sizeof(float));
    cout << w[1] << "   ";
    cout << w[2] << "   ";
    cout << w[3] << "   ";
    cout << w[4] << "   ";
    cout << w[5] << "   ";
    cout << w[13] << "   \n";
    
    myFile.read(reinterpret_cast< char *> (&w[1]), 13*sizeof(float));
    cout << w[1] << "   ";
    cout << w[2] << "   ";
    cout << w[3] << "   ";
    cout << w[4] << "   ";
    cout << w[5] << "   ";
    cout << w[13] << "   \n";
    myFile.read(reinterpret_cast< char *> (&w[1]), 13*sizeof(float));
    cout << w[1] << "   ";
    cout << w[2] << "   ";
    cout << w[3] << "   ";
    cout << w[4] << "   ";
    cout << w[5] << "   ";
    cout << w[13] << "   \n";
    
    cout << "Done!!!\n";
    
    return 0;
}

