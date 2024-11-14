#include<iostream>
using std::cout;
using std::cerr;
using std::ios;

#include <iomanip>
using std::setw;
using std::setprecision;
using std::scientific;
using std::fixed;


#include "constraint.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "solver.h"
//#include "../myheaders/matVecMult.h"


void ade1f(const posInt  &n,
           const posInt  &m,
           const posInt  &nec, 
           const posInt  &elementDofs,
           const posInt  &jo,
           const posInt  &nhip,
                 posInt  &lin,
                 posInt  &npag,
            const string title,                                                    
           const posInt  &icas,
           const posInt  &ncas,
           const string *fileName,
           const string *titleCase,
                 posInt  *leftCol,
                 real  *__restrict const loadVector,
                 real    **(&s))      
{
  //  Local Variables
  real **sm;
  real *forcesInElement;
  real *d;
  //  End of local variables

  // opening files for processing data
  elementsForcesBinaryFile.open(fileName[2].c_str(), ios::in | ios::out | ios::binary);
  if (!elementsForcesBinaryFile) {
    cerr << "File " << fileName[2] << " could not be opened.\n";
    exit(1);
  } // end if //

  // defining the size of the record for file elementsForcesBinaryFile
  const auto totalNumberOfForces = (secctionsInsideAnElement+2)*dofPerJoint;
  dimVec(forcesInElement,0,totalNumberOfForces+1); // forcesInElement[0] takes the place of the old tlar flag
  // end of defining the size of the record for file elementsForcesBinaryFile


  //  Creating arrays
  dimMat(sm,elementDofs,elementDofs);
  dimVec(d,totalNumberOfForces);
  //  end of Creating arrays


  //    impose boundary conditions on global load Vector

  jointRecNber=0;
  auto countingConstraints=zero;
  do  {
    jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);

    if (jointRecord.areThereRestrictions == true ) {
      ++countingConstraints;
      auto row = (jointRecNber)*dofPerJoint + 1;
      for(auto k=zero; k < dofPerJoint; ++k,++row) {
        if (constraint[jointRecNber][k] ) {
          loadVector[row]= 0.0;
        } // end if //
      } // end for //
    } // end if //
    ++jointRecNber;
  } while (countingConstraints < jo); // end do //

  // call subroutine ldl or cholesky to solve the system
  // of algebraic equation the solution is returned in the array glf
#if defined (LDL)
  cerr << "Solving Using LDL\n";
  ldl(cout,s, loadVector, nec,leftCol);     // Right-hand side solution
#elif defined (CHOLESKY)
  cerr << "Solving Using Cholesky\n";
  choles(cout,s, loadVector, nec,leftCol);  // Right-hand side solution
#else
  cerr << "Solving Using Conj. Grad.\n";
  conjGrad(cout,s,loadVector,nec,leftCol);
#endif

  // From now on loadVector has the global displacements
  if (lin + n + 2 > 56) {
    header(cout,title, lin, npag);
    cout << titleCase[icas] << "\n\n";
    lin+=2;
  } // end if //
  printJointInfo(4,lin);


  for(jointRecNber=0; jointRecNber<n; ++jointRecNber) {
    jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
    auto row = jointRecNber*dofPerJoint + 1;
    cout << setw(5) << jointRecNber+1 << setprecision(6);
    for(auto j=zero; j<dofPerJoint; ++j) {
      if (lin > 56) {
        header(cout,title, lin, npag);
        cout << titleCase[icas] << "\n\n";
        lin+=2;
        printJointInfo(4,lin);
      } // end if //
      //loadVector[row] += jointRecord.jointDispl[j];
      cout << setw(16) << scientific << (loadVector[row++] + jointRecord.jointDispl[j]);
      jointRecord.jointDispl[j]= 0.0;
    } // end for //
    jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.write(reinterpret_cast<const char *> (&jointRecord), sizeOfJointRecord);
    cout << '\n';
    ++lin;
  } // end for //
  cout << '\n';
  ++lin;
  if ((secctionsInsideAnElement == 0 and lin > 54) or (secctionsInsideAnElement > 0 and lin > 51) ) {
    header(cout,title, lin, npag);
    cout << titleCase[icas] << "\n\n";
    lin+=2;
  } // end if //
  printElementInfo(4,lin);
  for(elementRecNber=0; elementRecNber<m; ++elementRecNber) {
    if (secctionsInsideAnElement == 0 and lin > 56) {
      header(cout,title, lin, npag);
      cout << titleCase[icas] << "\n\n";
      lin+=2;
      printElementInfo(4,lin);
    } else if (secctionsInsideAnElement > 0 and lin > 53) {
      header(cout,title, lin, npag);
      cout << titleCase[icas] << "\n\n";
      lin+=2;
      printElementInfo(4,lin);
    } // end if //
    elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
    elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);


    // selecting global disp related to joints i and j in this element
    auto k =  (elementRecord.ji-1)*dofPerJoint;
    auto kk = (elementRecord.jj-1)*dofPerJoint;
    for(auto i=one; i<=dofPerJoint; ++i) {
      d[i] = loadVector[++k] ;
      d[i+dofPerJoint] = loadVector[++kk] ;
    } // end for //
    // end of selecting global disp related to joints i and j in this element


    real *temp;
    dimVec(temp, elementDofs);
    localElementStiffnessMatrix(sm, elementDofs, elementRecord);  // Calculo de la matriz de rigidez de miembro sm
    mult3(elementRecord,elementDofs, d,temp);                // Calculo de los displacements en coordenadas locales (d = r X D)
    mult5(sm,temp, d,elementDofs);        // Calculo de las fuerzas en coordenadas locales
    //    matVectMult(sm,temp, d,elementDofs);        // Calculo de las fuerzas en coordenadas locales
    freeVec(temp);

    if (nhip > 0) {
      elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
    } else {
      elementForcesRecNber=elementRecNber;
    } // end if //
    for(auto i=one*13; i<=totalNumberOfForces; ++i) {
      d[i] = 0.0;
    } // end for //
    elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
    elementsForcesBinaryFile.read(reinterpret_cast<char *> (forcesInElement), sizeOfElementForcesRecord);
    for(auto i=one; i<=totalNumberOfForces; ++i) {
      d[i] += forcesInElement[i];
    } // end for //
    d[1]  *= -1;
    d[4]  *= -1;
    d[6]  *= -1;
    d[8]  *= -1;
    d[9]  *= -1;
    d[11] *= -1;
    real segment = elementRecord.length/(secctionsInsideAnElement+1);
    for(auto i=one*12, ii=one; ii<=secctionsInsideAnElement; ++ii) {
      d[++i] += d[1];
      d[++i] += d[2];
      d[++i] += d[3];
      d[++i] += d[4];
      d[++i] += (d[5]+d[3]*ii*segment);
      d[++i] += (d[6]+d[2]*ii*segment);
    } // end for //

    cout << fixed <<setw(7) << elementRecNber+1 << setw(6) << elementRecord.ji << setw(8) << setprecision(3) << 0.0;
    for(auto i=one; i<=dofPerJoint; ++i) {
      cout << setw(11) << d[i];
    } // end for //
    cout << '\n';
    ++lin;
    if (secctionsInsideAnElement > 0) {
      auto kk = one * 12;
      for(auto k=one; k<=secctionsInsideAnElement; ++k) {
        cout << setw(21) << segment*k;
        for(auto i=one; i<=dofPerJoint; ++i) {
          cout <<  setw(11) << d[++kk];
        } // end for //
        cout << '\n';
      } // end for //
      lin += secctionsInsideAnElement;
    } // end if //
    cout << setw(13) << elementRecord.jj << setw(8)  << elementRecord.length;
    for(auto i=dofPerJoint+1; i<=2*dofPerJoint; ++i) {
      cout << setw(11) << d[i];
    } // end for //
    cout << '\n';
    ++lin;
    for(auto i=one; i<=totalNumberOfForces; ++i) {
      forcesInElement[i]=d[i];
    } // end for //
    elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
    elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
  } // end for //
  cout << '\n';
  ++lin;
  // Calculo de las reacciones
  for(auto i=one; i<=nec; ++i) {
    loadVector[i] = 0.0;
  } // end for //
  for(elementRecNber=0; elementRecNber<m; ++elementRecNber) {
    elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
    elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);

    if (nhip > 0) {
      elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
    } else {
      elementForcesRecNber=elementRecNber;
    } // end if //
    elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
    elementsForcesBinaryFile.read(reinterpret_cast<char *> (forcesInElement), sizeOfElementForcesRecord);

    forcesInElement[1]  *= -1;
    forcesInElement[4]  *= -1;
    forcesInElement[6]  *= -1;
    forcesInElement[8]  *= -1;
    forcesInElement[9]  *= -1;
    forcesInElement[11] *= -1;
    mult4(elementRecord,elementDofs,forcesInElement,d);
    // now d has the reactions in global coordinates

    // selecting global disp related to joints i and j in this element
    auto k=(elementRecord.ji-1)*dofPerJoint;
    auto kk=(elementRecord.jj-1)*dofPerJoint;
    for(auto i=one; i<=dofPerJoint; ++i) {
      loadVector[++k]  += d[i];
      loadVector[++kk] += d[i+dofPerJoint];
    } // end for //
    // end of selecting global disp related to joints i and j in this element
  } // end for //

  // opening files for processing data
  reactionsBinaryFile.open(fileName[3].c_str(),  ios::in | ios::out | ios::binary);
  if (!reactionsBinaryFile) {
    reactionsBinaryFile.open(fileName[3].c_str(),  ios::out | ios::binary);
    if (!reactionsBinaryFile) {
      cerr << "File " << fileName[3] << " could not be opened.\n";
      exit(1);
    } // end if //
  } // end if //

  real *jointReactionsRecord;
  jointReactionsRecord = new real [dofPerJoint];
  long sizeOfJointReactionsRecord=sizeof(real)*dofPerJoint;
  auto k = zero;
  jointReactionRecNber= static_cast<long>(icas-1);
  for(jointRecNber=0; jointRecNber<n; ++jointRecNber) {
    jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
    for(auto i=zero; i<dofPerJoint; ++i) {
      loadVector[++k] += jointRecord.load[i];
      jointReactionsRecord[i] = loadVector[k];
      jointRecord.load[i] = 0.0;
    } // end for //
    jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
    jointsBinaryFile.write(reinterpret_cast<const char *> (&jointRecord), sizeOfJointRecord);

    reactionsBinaryFile.seekp(sizeOfJointReactionsRecord*jointReactionRecNber,ios::beg);
    reactionsBinaryFile.write(reinterpret_cast<const char *> (jointReactionsRecord), sizeOfJointReactionsRecord);

    jointReactionRecNber+=ncas;
  } // end for //
  delete [] jointReactionsRecord;
  if (lin > 53) {
    header(cout,title, lin, npag);
    cout << titleCase[icas] << "\n\n";
    lin+=2;
  } // end if //
  printJointInfo(5,lin);
  k = 0;
  for(auto i=one; i<=n; ++i ) {
    if (lin > 56) {
      header(cout,title, lin, npag);
      cout << titleCase[icas] << "\n\n";
      lin+=2;
      printJointInfo(5,lin);
    } // end if //
    cout << setw(5) << i << setprecision(4);
    for(auto j=one; j<=dofPerJoint; ++j) {
      cout << setw(12) << loadVector[++k];
    } // end for //
    cout << '\n';
    ++lin;
  } // end for //
  freeMat(sm);
  freeVec(d);
  freeVec(forcesInElement,0);

  elementsForcesBinaryFile.close();
  reactionsBinaryFile.close();
  cerr << "End of Part ade1f \n";
} // end of ade1f() //
