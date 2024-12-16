#include<iostream>
using std::cerr;
using std::ios;

#include "constraint.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "solver.h"

#include "gaussElimination.h"



void ade1c(const posInt  &n,
        const posInt  &m,
        const posInt  &nec,              
        const posInt  &elementDofs,
        posInt  *leftCol,
        real    **(&s)) 
{
    //  Local Variables
    real **sm;
    posInt *destinationVector;
    //  End of local variables

    //  Creating the global stiffness ragged matrix
    dimMatVarBandW(s,nec,leftCol);
    dimMat(sm,elementDofs,elementDofs);
    dimVec(destinationVector,elementDofs);
    //  end of Creating the global stiffness ragged matrix

    for(elementRecNber=0; elementRecNber<m; ++elementRecNber) {
        elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
        elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);

        globalElementStiffnessMatrix(sm, elementDofs, elementRecord);

        // creating the destination Vector
        auto ji = (elementRecord.ji-1)*dofPerJoint;
        auto jj = (elementRecord.jj-1)*dofPerJoint;
        for(auto i=one; i<=dofPerJoint; ++i){
            destinationVector[i] = ++ji;
            if (constraint[elementRecord.ji-1][i-1] == true) {
                destinationVector[i] = 0;
            } // end if

            destinationVector[i + dofPerJoint] = ++jj;
            if (constraint[elementRecord.jj-1][i-1] == true) {
                destinationVector[i+ dofPerJoint] = 0;
            } // end if
        } // end for //
          // assembling the global stiffness matrix\n
        for(auto i=one; i<=elementDofs; ++i) {
            auto row = destinationVector[i];
            if (row > 0) {
                for(auto j=one; j<=i; ++j) {
                    auto col = destinationVector[j];
                    if (col > 0) {
                        if (row >= col) {
                            s[row][col] += sm[i][j];
                        } else {
                            s[col][row] += sm[i][j];
                        } // end if //
                    } // end if //
                } // end for //
            } // end if //
        } // end for //
    } // end for //


    // imposing boundary conditions on global stffness matrix
    for(jointRecNber=0; jointRecNber<n; ++jointRecNber) {
        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
        jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);

        if (jointRecord.areThereSprings == true) {
            auto row = (jointRecNber)*dofPerJoint + 1;
            for(auto k=zero; k < dofPerJoint; ++k,++row) {
                s[row][row] += jointRecord.springsConstants[k];
            } // end for //
        } // end if //
    } // end for //

    for(auto row=one; row<=nec; ++row) {
        if (s[row][row] == 0) {
            auto col = leftCol[row];
            while ((col<row) and (s[row][col] == 0.0)) {
                ++ col;
            } // end for //
            if (row == col) { // The whole row is empty
                s[row][col] = 1.0;
            } else {
                cerr << "Error in row: " << row << " of the srifness matrix. Quitting....\n" ;
                cout << "Error in row: " << row << " of the srifness matrix. Quitting....\n" ;
                exit(0);
            } // end if //
        } // end if //
    } // end for //

    // call subroutine ldl or cholesky to solve the system
    // of algebraic equation the solution is returned in the array glf
#if defined (LDL)
    cerr << "Solving Using LDL\n";
    ldl(cout,s,    nec,leftCol);     // Factorization of matrix stiff
#elif defined (CHOLESKY)
    cerr << "Solving Using Cholesky\n";
    choles(cout,s,    nec,leftCol);  // Factorization of matrix stiff
#endif

    freeMat(sm);
    freeVec(destinationVector);
    cerr << "End of Part ade1c \n";
} // end of ade1c() //
