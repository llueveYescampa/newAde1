#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <iomanip>
using std::setw;
using std::setprecision;

#include <cmath>

#include "language.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "dimen.h"


void ade1g(
        const posInt  &n,
        const posInt  &m,
        const posInt  &ncas,
        const posInt  &nhip,
        posInt  &nhipr,
        posInt  &lin,           
        posInt  &npag,
        const string title,                 
        const string *fileName,
        const string *titleCase,
        string *titleHip


        ) 

{
    //  Local Variables
    real **conv;
    real  **factor;
    real *forcesInElement;
    auto ncomb=zero;
    //  End of local variables


    // defining the size of the record for file elementsForcesBinaryFile
    const auto totalNumberOfForces = (secctionsInsideAnElement+2)*dofPerJoint;
    dimVec(forcesInElement,0,totalNumberOfForces+1); // forcesInElement[0] takes the place of the old tlar flag
                                                     // end of defining the size of the record for file elementsForcesBinaryFile


                                                     // opening files for processing data
    elementsForcesBinaryFile.open(fileName[2].c_str(), ios::in  | ios::binary);
    if (!elementsForcesBinaryFile) {
        cerr << "File " << fileName[2] << " could not be opened.\n";
        exit(1);
    } // end if //

    dimMat(factor,nhip,ncas);

    header(cout,title,lin,npag);

    if (nhip>0) {
        cin.ignore(1000,'\n');
        getline(cin,titleHip[0]);
        cout << '\n';  
        cout << titleHip[0];    
#ifdef ENGLISH
        cout << "\nHypothesis   Load Case    Factor\n";
#endif  
#ifdef ESPANOL  
        cout << "\nHipotesis  Caso de Carga  Factor\n";
#endif      
        lin+=3;

        for(auto i=one; i<=nhip; ++i) {
            auto k=zero;
            cin >> k >> ncomb;
            cout << setw(5) << k;
            for(auto j=one;j<=ncomb; ++j) {
                auto kk=zero;
                cin >> kk;
                if (j==1) {
                    cout << setw(13);
                } else {
                    cout << setw(18);
                } // end if //
                cin >> factor[k][kk];
                cout << kk  << setw(14) << factor[k][kk] << '\n';
                ++lin;
            } // end for //
            cout << '\n';
            ++lin;
        } // end for //
    } // end if //

    header(cout,title,lin,npag);
    printElementInfo(5, lin);
    printElementInfo(6, lin);

    elementForcesRecNber=0;
    for(auto elementRecNber=zero; elementRecNber<m; ++elementRecNber) {
        dimMat(conv,nhip,totalNumberOfForces);
        for(auto k =one; k<=ncas; ++k) {
            elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber++,ios::beg);
            elementsForcesBinaryFile.read(reinterpret_cast<char *> (&forcesInElement[0]), sizeOfElementForcesRecord);
            // reordering  forcesInElement[] arrray for easy printing
            real *temp;
            dimVec(temp,dofPerJoint);
            for(auto i=one,j=dofPerJoint+1; i<=dofPerJoint; ++i,++j) {
                temp[i] = forcesInElement[j];
            } // end for //

            for(auto i=2*dofPerJoint+1, j=dofPerJoint+1; i<=totalNumberOfForces; ++i,++j) {
                forcesInElement[j] = forcesInElement[i];
            } // end for //

            for(auto i=one, j=totalNumberOfForces-dofPerJoint; i<=dofPerJoint; ++i) {
                forcesInElement[++j] = temp[i];
            } // end for //
            freeVec(temp);
            // end of reordering  forcesInElement[] arrray for easy printing

            for(auto j=one; j<=nhip; ++j) {
                for(auto i=one; i<=totalNumberOfForces; ++i) {
                    conv[j][i] += forcesInElement[i]*factor[j][k];
                } // end for //
            } // end for //
        } // end for //

        if (secctionsInsideAnElement>0 and lin > 56 - (secctionsInsideAnElement+2)*nhip) {
            header(cout,title,lin,npag);
            printElementInfo(6, lin);
        } else if (secctionsInsideAnElement == 0 and lin > (56 -2*nhip)){
            header(cout,title,lin,npag);
            printElementInfo(6, lin);
        } // end if //

        elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
        elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);
        real segment = elementRecord.length/(secctionsInsideAnElement+1);
        real position = 0.0;
        cout << setw(7) <<  elementRecNber+1 << setprecision(3) ;
        for(auto jj=one; jj<=secctionsInsideAnElement+2; ++jj) {
            auto i = (jj-1)*dofPerJoint;
            if (jj==1) {
                cout  << setw(8) << position;
            } else {
                cout  << setw(15)  << position;
            } // end if
            position += segment;
            for(auto j=one; j<=nhip; ++j) {
                if (j==1) {
                    cout << setw(11) << j;
                } else {
                    cout << setw(26) << j;
                } // end if

                for(auto k=one; k<=dofPerJoint; ++k) {
                    cout << setw(12) << conv[j][++i];
                } // end for //
                i -= dofPerJoint;
                cout << '\n';
            } // end for //
        } // end for //
        lin+=(secctionsInsideAnElement+2)*nhip;
        freeMat(conv);
    } // end for //

    freeVec(forcesInElement,0);
    elementsForcesBinaryFile.close();

    header(cout,title,lin,npag);


    if (nhipr == 0) { // take nhipr = nhip;
#ifdef ENGLISH
        titleHip[1]="Using the same combinations of the elements";
#endif  
#ifdef ESPANOL  
        titleHip[1]="Usando las mismas combinaciones de los miembros";    
#endif  
        nhipr=nhip;
        cout << '\n';
        cout << titleHip[1];
#ifdef ENGLISH
        cout << "\nHypothesis   Load Case    Factor\n";
#endif  
#ifdef ESPANOL  
        cout << "\nHipotesis  Caso de Carga  Factor\n";
#endif  
        lin+=3;

        for(auto i=one; i<=nhip; ++i) {
            cout << setw(5) << i;
            for(auto j=one;j<=ncas; ++j) {
                if (j==1) {
                    cout << setw(13);
                } else {
                    cout << setw(18);
                } // end if //
                if (factor[i][j] != 0.0)
                    cout << j << setw(14) << factor[i][j] << '\n';
                ++lin;
            } // end for //
            cout << '\n';
            ++lin;
        } // end for //
    } else {
        freeMat(factor);
        dimMat(factor,nhipr,ncas);
        cin.ignore(1000,'\n');
        getline(cin,titleHip[1]);
        cout << '\n';
        cout << titleHip[1];
#ifdef ENGLISH
        cout << "\nHypothesis   Load Case    Factor\n";
#endif  
#ifdef ESPANOL  
        cout << "\nHipotesis  Caso de Carga  Factor\n";
#endif  
        lin+=3;

        for(auto i=one; i<=nhipr; ++i) {
            auto k = zero;
            cin >> k  >> ncomb;
            cout << setw(5) << k;
            for(auto j=one;j<=ncomb; ++j) {
                auto kk = zero;
                cin >> kk;
                cin >> factor[k][kk];
                if (j==1) {
                    cout << setw(13);
                } else {
                    cout << setw(18);
                } // end if //
                cout << kk << setw(14) << factor[k][kk] << '\n';
                ++lin;
            } // end for //
            cout << '\n';
            ++lin;
        } // end for //
    } // end if //

    header(cout,title,lin,npag);
    printJointInfo(6, lin);

    // opening files for processing data
    reactionsBinaryFile.open(fileName[3].c_str(), ios::in | ios::binary);
    if (!reactionsBinaryFile) {
        cerr << "File " << fileName[3] << " could not be opened.\n";
        exit(1);
    } // end if //


    jointReactionRecNber= 0;
    real *jointReactionsRecord;
    jointReactionsRecord = new real [dofPerJoint];
    long sizeOfJointReactionsRecord=sizeof(real)*dofPerJoint;
    for(jointRecNber=0; jointRecNber<n; ++jointRecNber) {
        dimMat(conv,nhipr,dofPerJoint);  // just to have a clean conv array
        for(auto k=one; k<=ncas; ++k) {
            //cerr << "position: " << jointReactionRecNber << "   ";
            reactionsBinaryFile.seekg(sizeOfJointReactionsRecord*jointReactionRecNber++,ios::beg);
            reactionsBinaryFile.read(reinterpret_cast<char *> (jointReactionsRecord), sizeOfJointReactionsRecord);
            for(auto j=one; j<=nhipr; ++j) {
                for(auto i=zero; i<dofPerJoint; ++i) {
                    conv[j][i+1] += jointReactionsRecord[i]*factor[j][k];
                } // end for //
            } // end for //
        } // end for //

        if (lin > (56 -nhipr)) {
            header(cout,title,lin,npag);
            printJointInfo(6, lin);
        } // end if //
        cout << setw(5) << jointRecNber+1 ;
        for(auto j=one; j<=nhipr; ++j) {
            if (j==1) {
                cout << setw(6) << j;
            } else {
                cout << setw(11) << j;
            } // end if //
            for(auto k=one; k<=dofPerJoint; ++k) {
                cout << setw(12) << conv[j][k];
            } // end for //
            cout << '\n';
        } // end for //
        ++lin;
        freeMat(conv);
    } // end for //
    delete [] jointReactionsRecord;

    freeMat(factor);
    reactionsBinaryFile.close();
    cerr << "End of Part ade1g \n";
} // end of ade1g() //
