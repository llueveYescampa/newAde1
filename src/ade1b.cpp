#include<iostream>
using std::cout;
using std::cerr;
using std::ios;

#include <fstream>
using std::ifstream;


#include <iomanip>
using std::setw;
using std::fixed;
using std::setprecision;
using std::scientific;
using std::uppercase;

#include "language.h"
#include "constraint.h"
#include "prototyp.h"
#include "joint.h"
//#include "element.h"


void ade1b( const posInt  &n,
        const posInt  &m,
        const posInt  &nec,
        const posInt  &ncas, 
        const posInt  &jo,
        const posInt  &nmat, 
        const posInt  &nhip, 
        const posInt  &nhipr,
        const posInt  *__restrict const leftCol,
        posInt  &lin,
        posInt  &npag,
        const string title,                  
        const real  *__restrict const e,
        const real  *__restrict const g,
        const real  *__restrict const ct,
        const real  *__restrict const pes,
        fstream &elementsBinaryFile) 
{
    //  Local Variables
    real vol=0.0, weight=0.0;
    //  End of local variables


    header(cout,title, lin, npag);

    cout << "\nN=" << setw(3) << n     << setw(12)
        << "JO=" <<  setw(3) << jo    << setw(12)
        << "M="  <<  setw(3)  << m    << setw(12)
        << "NCAS=" << setw(3) << ncas << setw(12)
        << "NMAT=" << setw(3) << nmat << setw(12)
        << "NHIP=" << setw(3) << nhip << setw(12)
        << "NHIPR=" << setw(3) << ((nhipr==0) ? nhip:nhipr) << "\n\n";

#ifdef ENGLISH
    cout <<   "Properties of the material types.\n";
    cout <<   "Type        E             G             Ct          Weight\n";
#endif  

#ifdef ESPANOL  
    cout <<   "Propiedades de los tipos de materiales.\n";
    cout <<   "Tipo        E             G             Ct            Peso\n";
#endif  

    for(auto i=one; i<=nmat; ++i){
        cout << i << setw(12) << e[i] << setw(14) << g[i] << setw(15) << ct[i] << setw(16) << pes[i] << '\n';
    } // end for //
    cout << '\n';
    lin += (nmat+5);
    if (lin+n > 56){
        header(cout,title, lin, npag);
    } // end if //
    printJointInfo(1,lin);
    for(jointRecNber=0; jointRecNber<n; ++jointRecNber) {
        if (lin > 56) {
            header(cout,title, lin, npag);
            printJointInfo(1,lin);
        } // end if //
        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
        jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);

        cout << fixed;
        //cout.precision(3);
        //cout.precision(3);
        cout << setw(5) << jointRecNber+1
            << setw(5) << constraint[jointRecNber][0]
            << setw(5) << constraint[jointRecNber][1]
            << setw(5) << constraint[jointRecNber][2]
            << setw(5) << constraint[jointRecNber][3]
            << setw(5) << constraint[jointRecNber][4]
            << setw(5) << constraint[jointRecNber][5] << setprecision(3)
            << setw(10) << jointRecord.x
            << setw(10) << jointRecord.y
            << setw(10) << jointRecord.z << setprecision(4) << scientific // << setprecision(0)
            << setw(12) << jointRecord.springsConstants[0]
            << setw(11) << jointRecord.springsConstants[1]
            << setw(11) << jointRecord.springsConstants[2]
            << setw(11) << jointRecord.springsConstants[3]
            << setw(11) << jointRecord.springsConstants[4]
            << setw(11) << jointRecord.springsConstants[5] << fixed << '\n';
        ++lin;
    } // end for //
    cout << '\n';
    ++lin;
    if (lin+m > 56) {
        header(cout,title, lin, npag);
    } // end if //
    printElementInfo(1,lin);
    for(elementRecNber=0; elementRecNber<m; ++elementRecNber) {
        if (lin > 56) {
            header(cout,title, lin, npag);
            printElementInfo(1,lin);
        } // end if //
        elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
        elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);


        real eOverLength = e[elementRecord.materialType] / elementRecord.length;
        real ar = elementRecord.ar/eOverLength;
        real ay = elementRecord.ay/eOverLength;
        real az = elementRecord.az/eOverLength;
        real ax = elementRecord.ax  * elementRecord.length/g[elementRecord.materialType];
        vol += elementRecord.length*ar;
        weight += pes[elementRecord.materialType]*elementRecord.length*ar;
        int ji = elementRecord.ji;
        int jj = elementRecord.jj;

        if(elementRecord.iar > 0) {
            if(elementRecord.iar==1 || elementRecord.iar==3) {
                ji *= -1;
            } // end if //
            if(elementRecord.iar==2 || elementRecord.iar==3) {
                jj *= -1;
            } // end if //
        } // end if //
        cout << setw(4) << elementRecNber+1
            << setw(4) << ji
            << setw(4) << jj << setprecision(2)
            << setw(7) << elementRecord.length << uppercase <<scientific  << setprecision(3)
            << setw(11) << ar
            << setw(11) << ax
            << setw(11) << ay
            << setw(11) << az << fixed << setprecision(2)
            << setw(7) << elementRecord.stiffnessData[4]
            << setw(7) << elementRecord.stiffnessData[5]
            << setw(7) << elementRecord.stiffnessData[6]
            << setw(6) << elementRecord.stiffnessData[0]
            << setw(7) << elementRecord.stiffnessData[1]
            << setw(7) << elementRecord.stiffnessData[2]
            << setw(7) << elementRecord.stiffnessData[3]
            << setw(5) << elementRecord.materialType;


        if(elementRecord.bz > 0) {
            cout << setw(8) << elementRecord.bz;
        } // end if//
        if(elementRecord.by > 0) {
            cout << setw(7) << elementRecord.by;
        } // end if//
        cout << '\n';

        /*
           if(elementRecord.bz > 0) {
           cout << setw(8) << elementRecord.bz << setw(7) << elementRecord.by << '\n';
           } else {
           cout << '\n';
           } // end if //
         */

        ++lin;
    } // end for //
    real meanBW=0.0;
    // Calculo de la media-banda promedio
    for(posInt i = 1; i <= nec; ++i) {
        meanBW += (i-leftCol[i]);
    } // endfor //
    meanBW += nec;
    meanBW /= nec;
    // end of Calculo de la media-banda promedio


    //cout.precision(3);
#ifdef ENGLISH  
    cout << setprecision (3) <<"\nVolume  = " << setw(6) << vol << setw(30) << "Weight=" << setw(6) << weight << setw(40) << "Mean BandWidth: " << meanBW << "\n\n";
#endif  
#ifdef ESPANOL  
    cout << setprecision (3) <<"\nVolumen = " << setw(6) << vol << setw(30) << "Peso =" << setw(6) << weight << setw(40) << "Ancho de banda promedio: " << meanBW << "\n\n";
#endif  

    lin += 3;
    cerr << "End of Part ade1b \n";
} // end of ade1b() //
