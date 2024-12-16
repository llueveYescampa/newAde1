#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <iomanip>
using std::setw;
using std::setprecision;
using std::fixed;

#include "language.h"
#include "constraint.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "dimen.h"


void ade1d(const posInt  &m,               
        const posInt  &nec, 
        const posInt  &elementDofs,             
        const posInt  &nhip,
        posInt  &njc,
        posInt  &nma,
        posInt  &nmc,
        posInt  &icas,
        posInt  &ncas,
        posInt  &lin,                 
        posInt  &npag,
        const string title,                                   
        string *fileName,
        string *titleCase,
        real    &ato,
        const real  *__restrict const ct,
        real  *__restrict const loadVector

        ) 

{
    //  Local Variables
    real **sm;
    real *forcesInElement;
    real *d;
    //  End of local variables


    // opening files for processing data
    elementsForcesBinaryFile.open(fileName[2].c_str(), ios::in|ios::out|ios::binary);
    if (!elementsForcesBinaryFile) {
        elementsForcesBinaryFile.open(fileName[2].c_str(), ios::out|ios::binary);
        if (!elementsForcesBinaryFile) {
            cerr << "File " << fileName[2] << " could not be opened.\n";
            exit(1);
        } // end if //
    } // end if //


    // defining the size of the record for file elementsForcesBinaryFile
    auto totalNumberOfForces = (secctionsInsideAnElement+2)*dofPerJoint;
    dimVec(forcesInElement,0,totalNumberOfForces+1); // forcesInElement[0] takes the place of the old tlar flag
    sizeOfElementForcesRecord = (totalNumberOfForces+1)*sizeof(real);
    // end of defining the size of the record for file elementsForcesBinaryFile


    //  Creating the element stiffness matrix
    dimVec(d,elementDofs);
    dimMat(sm,elementDofs,elementDofs);
    //  end of Creating the element stiffness matrix


    ++icas;
    if (icas > 1 ) {
        for(auto i=one; i<=nec; ++i) {
            loadVector[i] = 0.0;
        } // end for //
    } // end if //

    header(cout,title, lin, npag);
    cin.ignore(1000,'\n');
    getline(cin,titleCase[icas]);
    cout << titleCase[icas] << "\n\n";
    lin+=2;


    cin >> njc >> nma >> nmc >> ato;

#ifdef ENGLISH
    cout << fixed 
        << "NNL = " << setw(4) << njc << setw(8)
        << "NSD = " << setw(4) << nma << setw(8)
        << "NEL = " << setw(4) << nmc << setw(8)
        << "GDT = " << setw(6) << setprecision (3) << ato << "\n\n";
#endif  
#ifdef ESPANOL  
    cout << fixed 
        << "NJC = " << setw(4) << njc << setw(8)
        << "NMA = " << setw(4) << nma << setw(8)
        << "NMC = " << setw(4) << nmc << setw(8)
        << "ATO = " << setw(6) << setprecision (3) << ato << "\n\n";
#endif  

    lin +=2;
    forcesInElement[0] = static_cast<real>(false); // redundant because was created as 0

    if (nhip == 0) {
        for(auto i=zero; i<m; ++i) {
            elementForcesRecNber=i;
            elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
        } // end for //
    } else {
        for(auto i=zero; i<m; ++i) {
            elementForcesRecNber = (icas -1) + ncas * (i);
            elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
        } // end for //
    } // end if //

    // Determinacion de la contribucion de las cargas aplicadas
    // en los nodos al vector de cargas loadVector[]

    if (njc > 0) {
        printJointInfo(2,lin);
        for(auto i=one; i<=njc; ++i) {
            if (lin > 56) {
                header(cout,title, lin, npag);
                cout << titleCase[icas] << "\n\n";
                lin+=2;
                printJointInfo(2,lin);
            } // end if //

            auto j=zero;
            cin >> j;
            jointRecNber= j-1; // = static_cast<long>(w[1]) - 1;
            cout << setw(5) << j;

            auto k=(j-1)*dofPerJoint;
            for(auto ii=one; ii<=dofPerJoint; ++ii) {
                cin >> loadVector[++k];
                cout << setw(11) << setprecision(3) << loadVector[k]; //w[ii];
            } // end for //
            cout << '\n';
            ++lin;
            jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
            jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);


            if (jointRecord.areThereRestrictions) {
                k=(j-1)*dofPerJoint+1;
                for(auto i=zero; i<dofPerJoint; ++i, ++k) {
                    if (constraint[jointRecNber][i] or jointRecord.springsConstants[i] > 0.0 ) {
                        jointRecord.load[i] = -loadVector[k];
                        //} else {
                        //   jointRecord.load[i] = 0.0;
                } // end if //
                } // end for //

                jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
                jointsBinaryFile.write(reinterpret_cast<const char *> (&jointRecord), sizeOfJointRecord);
            } // end if //
        } // end for //
        cout << '\n';
        ++lin;
    } // end if //


    // Desplazamientos de apoyos
    if (lin + nma + 2 > 56) {
        header(cout,title, lin, npag);
        cout << titleCase[icas] << "\n\n";
        lin+=2;
    } // end if //
    if (nma > 0) {
        printJointInfo(3,lin);
        for(auto i=one; i<=nma; ++i) {
            if (lin > 56) {
                header(cout,title, lin, npag);
                cout << titleCase[icas] << "\n\n";
                lin+=2;
                printJointInfo(3, lin);
            } // end if //
            auto j=zero;
            cin >> j;
            jointRecNber = j-1;

            //cerr << j << "    " << w[2] << "   " << w[3] << "   " << w[4]<< "   " << w[5]<< "   " << w[6] << "   " << w[7] << '\n';
            jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
            jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);

            if (jointRecord.areThereRestrictions == false) {
#ifdef ENGLISH
                cerr << "ERROR. Support displacement at unrestricted node.\n";
                cerr << "Please, chech node: " << j << '\n';
                cout << "ERROR. Support displacement at unrestricted node.\n";
                cout << "Please, chech node: " << j << '\n';
#endif  
#ifdef ESPANOL  
                cerr << "ERROR. Movimiento de apoyo en junta no restringida.\n";
                cerr << "Revise la junta : " << j << '\n';
                cout << "ERROR. Movimiento de apoyo en junta no restringida.\n";
                cout << "Revise la junta : " << j << '\n';
#endif  
                exit(0);
            } // end if //


            cout << setw(5) << j;
            for(auto k=zero; k<dofPerJoint; ++k) {
                cin >> jointRecord.jointDispl[k];
                cout << setw(11) << setprecision(7) <<jointRecord.jointDispl[k];
                if ( !constraint[jointRecNber][k]  and jointRecord.jointDispl[k] != 0.0) {
#ifdef ENGLISH
                    cerr << "ERROR. Support displacement at unrestricted dof.\n";
                    cerr << "Please, chech node: " << j << '\n';
                    cout << "ERROR. Support displacement at unrestricted dof.\n";
                    cout << "Please, chech node: " << j << '\n';
#endif  
#ifdef ESPANOL  
                    cerr << "ERROR. Movimiento de apoyo en coordenada no restringida.\n";
                    cerr << "Revise la junta : " << j << '\n';
                    cout << "ERROR. Movimiento de apoyo en coordenada no restringida.\n";
                    cout << "Revise la junta : " << j << '\n';
#endif  
                    exit(0);
                } // end if //
            } // end for //
            jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
            jointsBinaryFile.write(reinterpret_cast<const char *> (&jointRecord), sizeOfJointRecord);
            cout << '\n';
            ++lin;
        } // end for //
        cout << '\n';
        ++lin;
        for(elementRecNber=0; elementRecNber < m; ++elementRecNber) {
            elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
            elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);
            bool mytest = false;
            jointRecNber = static_cast<long>(elementRecord.ji)-1 ;
            // Determinacion de desplazamientos en el miembro [i]
            for(posInt k=1,j=1; j<=2; ++j) {
                jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
                jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
                for(auto i=zero; i<dofPerJoint; ++i,++k) {
                    d[k] = jointRecord.jointDispl[i];
                    if (d[k] != 0.0) {
                        mytest = true;
                    } // end if //
                } // end for //
                jointRecNber = static_cast<long>(elementRecord.jj)-1 ;
            } // end for //
            if (mytest) { // Solo se ejecuta si el miembro tiene desplazamiento en alguno de sus extremos
                real *temp;
                dimVec(temp, elementDofs);
                localElementStiffnessMatrix(sm,elementDofs, elementRecord);  // Calculo de la matriz de rigidez de miembro sm
                mult3(elementRecord,elementDofs,d,temp);                // Calculo de los desplazamientos en coordenadas locales (d = r X D)
                mult5(sm,temp, d,elementDofs);              // Calculo de las fuerzas primarias debido a movimientos de apoyos
                                                            //matVectMult(sm,temp, d,elementDofs);        // Calculo de las fuerzas primarias debido a movimientos de apoyos
                freeVec(temp);
                if (nhip > 0) {
                    elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
                } else {
                    elementForcesRecNber=elementRecNber;
                } // end if //
                forcesInElement[0] = 1.0;  // Flag to indicate that there is primary forces in this member
                for(auto k=one; k<=elementDofs; ++k) {
                    forcesInElement[k] = d[k];
                } // end if //
                elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
                elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
            } // end if //
        } // end for //
    } // end if //
    if (ato != 0) { // Determinacion de la contribucion de los cambios de temperaturas a las cargas primarias
        for(elementRecNber=0; elementRecNber < m; ++elementRecNber) {
            elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
            elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);
            d[1] = elementRecord.ar*elementRecord.length*ct[elementRecord.materialType]*ato;
            if (nhip > 0) {
                elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
            } else {
                elementForcesRecNber=elementRecNber;
            } // end if //
            elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.read(reinterpret_cast<char *> (forcesInElement), sizeOfElementForcesRecord);
            forcesInElement[0] = 1.0;  // Flag to indicate that there is primary forces in this member
            forcesInElement[1]+=d[1];
            forcesInElement[7]-=d[1];
            elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
        } // end for //
    } // end if //
    freeVec(forcesInElement,0);
    freeVec(d);
    freeMat(sm);
    elementsForcesBinaryFile.close();
    cerr << "End of Part ade1d \n";
} // end of ade1d() //
