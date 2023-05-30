#include<iostream>
using std::cerr;
using std::ios;

#include "common.h"
#include "constraint.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "solver.h"

#include "gaussElimination.h"



void ade1c() {
  //  Local Variables
  real **sm;
  size_t *destinationVector;
  //  End of local variables

  //  Creating the global stiffness ragged matrix
  dimMatVarBandW(s,nec,leftCol);
  dimMat(sm,elementDofs,elementDofs);
  dimVec(destinationVector,elementDofs);
  //  end of Creating the global stiffness ragged matrix

  for(elementRecNber=0; elementRecNber<m; ++elementRecNber) {
    elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
    elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);

    globalElementStiffnessMatrix(sm, elementRecord);

    // creating the destination Vector
    size_t ji = (elementRecord.ji-1)*dofPerJoint;
    size_t jj = (elementRecord.jj-1)*dofPerJoint;
    for (size_t i=1; i<=dofPerJoint; ++i){
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
    for (size_t i=1; i<=elementDofs; ++i) {
      size_t row = destinationVector[i];
      if (row > 0) {
        for (size_t j=1; j<=i; ++j) {
          size_t col = destinationVector[j];
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
  for (jointRecNber=0; jointRecNber<n; ++jointRecNber) {
    jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);

    if (jointRecord.areThereSprings == true) {
      size_t row = (jointRecNber)*dofPerJoint + 1;
      for (size_t k=0; k < dofPerJoint; ++k,++row) {
        s[row][row] += jointRecord.springsConstants[k];
      } // end for //
    } // end if //
  } // end for //



  for (size_t row=1; row<=nec; ++row) {
    if (s[row][row] == 0) {
      size_t col = leftCol[row];
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
