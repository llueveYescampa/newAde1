#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <iomanip>
using std::setw;
using std::setprecision;

#include <cmath>

#include "common.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"


void ade1g() {
  //  Local Variables
  real **conv;
  real  **factor;
  real *forcesInElement;
  size_t ncomb;
  //  End of local variables


  // defining the size of the record for file elementsForcesBinaryFile
  const size_t totalNumberOfForces = (secctionsInsideAnElement+2)*dofPerJoint;
  dimVec(forcesInElement,0,totalNumberOfForces+1); // forcesInElement[0] takes the place of the old tlar flag
  // end of defining the size of the record for file elementsForcesBinaryFile


  // opening files for processing data
  elementsForcesBinaryFile.open(fileName[2].c_str(), ios::in  | ios::binary);
  if (!elementsForcesBinaryFile) {
    cerr << "File " << fileName[2] << " could not be opened.\n";
    exit(1);
  } // end if //

  dimMat(factor,nhip,ncas);

  header(cout);
  cout << '\n';
  cout << titleHip[0];
  cout << "\nHipotesis  Caso de Carga  Factor\n";
  lin+=3;

  if (nhip>0) {
    cin.ignore(1000,'\n');
    getline(cin,titleHip[0]);
    for (size_t i=1; i<=nhip; ++i) {
      size_t  k;
      cin >> k >> ncomb;
      cout << setw(5) << k;
      for (size_t j=1;j<=ncomb; ++j) {
        size_t kk;
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

  header(cout);
  printElementInfo(5, lin);
  printElementInfo(6, lin);

  elementForcesRecNber=0;
  for (size_t elementRecNber=0; elementRecNber<m; ++elementRecNber) {
    dimMat(conv,nhip,totalNumberOfForces);
    for (size_t k =1; k<=ncas; ++k) {
      elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber++,ios::beg);
      elementsForcesBinaryFile.read(reinterpret_cast<char *> (&forcesInElement[0]), sizeOfElementForcesRecord);
      // reordering  forcesInElement[] arrray for easy printing
      real *temp;
      dimVec(temp,dofPerJoint);
      for (size_t i=1,j=dofPerJoint+1; i<=dofPerJoint; ++i,++j) {
        temp[i] = forcesInElement[j];
      } // end for //

      for (size_t i=2*dofPerJoint+1, j=dofPerJoint+1; i<=totalNumberOfForces; ++i,++j) {
        forcesInElement[j] = forcesInElement[i];
      } // end for //

      for (size_t i=1, j=totalNumberOfForces-dofPerJoint; i<=dofPerJoint; ++i) {
        forcesInElement[++j] = temp[i];
      } // end for //
      freeVec(temp);
      // end of reordering  forcesInElement[] arrray for easy printing

      for (size_t j=1; j<=nhip; ++j) {
        for (size_t i=1; i<=totalNumberOfForces; ++i) {
          conv[j][i] += forcesInElement[i]*factor[j][k];
        } // end for //
      } // end for //
    } // end for //

    if (secctionsInsideAnElement>0 and lin > 56 - (secctionsInsideAnElement+2)*nhip) {
      header(cout);
      printElementInfo(6, lin);
    } else if (secctionsInsideAnElement == 0 and lin > (56 -2*nhip)){
      header(cout);
      printElementInfo(6, lin);
    } // end if //

    elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
    elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);
    real segment = elementRecord.length/(secctionsInsideAnElement+1);
    real position = 0.0;
    cout << setw(7) <<  elementRecNber+1 << setprecision(3) ;
    for (size_t jj=1; jj<=secctionsInsideAnElement+2; ++jj) {
      size_t i = (jj-1)*dofPerJoint;
      if (jj==1) {
        cout  << setw(8) << position;
      } else {
        cout  << setw(15)  << position;
      } // end if
      position += segment;
      for (size_t j=1; j<=nhip; ++j) {
        if (j==1) {
          cout << setw(11) << j;
        } else {
          cout << setw(26) << j;
        } // end if

        for (size_t k=1; k<=dofPerJoint; ++k) {
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

  header(cout);
  cout << '\n';
  cout << titleHip[1];
  cout << "\nHipotesis  Caso de Carga  Factor\n";
  lin+=3;


  if (nhipr == 0) { // take nhipr = nhip;
    titleHip[1]="Usando las mismas combinaciones de los miembros";
    nhipr=nhip;
    for (size_t i=1; i<=nhip; ++i) {
      cout << setw(5) << i;
      for (size_t j=1;j<=ncas; ++j) {
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
    for (size_t i=1; i<=nhipr; ++i) {
      size_t  k;
      cin >> k  >> ncomb;
      cout << setw(5) << k;
      for (size_t j=1;j<=ncomb; ++j) {
        size_t kk;
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

  header(cout);
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
    for (size_t k =1; k<=ncas; ++k) {
      //cerr << "position: " << jointReactionRecNber << "   ";
      reactionsBinaryFile.seekg(sizeOfJointReactionsRecord*jointReactionRecNber++,ios::beg);
      reactionsBinaryFile.read(reinterpret_cast<char *> (jointReactionsRecord), sizeOfJointReactionsRecord);
      for (size_t j=1; j<=nhipr; ++j) {
        for (size_t i=0; i<dofPerJoint; ++i) {
          conv[j][i+1] += jointReactionsRecord[i]*factor[j][k];
        } // end for //
      } // end for //
    } // end for //

    if (lin > (56 -nhipr)) {
      header(cout);
      printJointInfo(6, lin);
    } // end if //
    cout << setw(5) << jointRecNber+1 ;
    for (size_t j=1; j<=nhipr; ++j) {
      if (j==1) {
        cout << setw(6) << j;
      } else {
        cout << setw(11) << j;
      } // end if //
      for (size_t k=1; k<=dofPerJoint; ++k) {
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
