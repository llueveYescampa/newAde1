#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;


#include <iomanip>
using std::setw;
using std::fixed;
using std::setprecision;

#include <cstring>
using std::string;

#include <filesystem>
using std::filesystem::path;
using std::filesystem::remove;

#include <chrono>
using ns = std::chrono::nanoseconds;


#include "newade1.h"
#include "prototyp.h"
#include "dimen.h"

int main(int argc, char *argv[])
{

  //  Local Variables
    real    *loadVector, **s;
    posInt  nec, *leftCol;
    posInt  n, jo, m, ncas, nhip, nhipr, itipo, npag=0;
    posInt  nmc, njc, nma, icas=0, lin, totalNbrOfSprings=0;
    string  title,*titleCase,*titleHip, *fileName;
    bool    reviewData;
    int     nmat;
    real *e, *g, *ct, *pes, ato;
    constexpr posInt  elementDofs = 2*dofPerJoint;
    bool    considerShearDef=false;



  int mm;
  //  End of local variables

  // Input and output streams
  ifstream infile;
  ofstream outfile;
  // end of Input and output streams

  if (argc == 1 ) {
    cout << "Use: " << argv[0] << " inputFile [outputFile]" << endl;
    exit(0);
  } else if (argc > 1 ) {
    infile.open(argv[1], ios::in);
    if(infile.fail()) {
        cerr << "Input File: " << argv[1] << " do not exist!\n"; 
        exit(0);
    } // end if //
    std2file(cin,infile);
    if (argc >= 3) {
      outfile.open(argv[2], ios::out);
      std2file(cout,outfile);
    } // endif //
  } // endif //
  path p = argv[1];

    // assigning file names
  fileName = new string[4];
  //fileName[0] = strtok(argv[1],".");
  fileName[0] = p.replace_extension();
  fileName[1] = fileName[0] + ".elm";
  fileName[2] = fileName[0] + ".efs";
  fileName[3] = fileName[0] + ".rfs";
  fileName[0] += ".jnt";
  // end of assigning file names

  jointsBinaryFile.open(fileName[0].c_str(),  ios::in | ios::out | ios::trunc | ios::binary);
  if (!jointsBinaryFile) {
    cerr << "File " <<  fileName[0] << " could not be opened.\n";
    exit(1);
  } // end if //

  elementsBinaryFile.open(fileName[1].c_str(),  ios::in | ios::out | ios::trunc | ios::binary);
  if (!elementsBinaryFile) {
    cerr << "File " <<    fileName[1] << " could not be opened.\n";
    exit(1);
  } // end if //

  // end of opening files for processing data

  // R e a d i n g    t h e    i n p u t    d a t a    h e r e
  getline(cin,title);
  header(cerr, title, lin, npag);
  //start = clock();
  auto start = std::chrono::steady_clock::now();

  cin >> n >> jo >>  mm >> ncas >> nmat >> nhip >> nhipr >> itipo >> reviewData;
  if (nmat < 0) { // consider shear deff. if the number o materials is negative
    nmat = -nmat;
    considerShearDef=true;
  } // end if //
  nec = dofPerJoint * n;  // total number of equation to solve
  if(ncas < 1) {
    ncas = 1; // at least one case
  } // end if //
  if(mm < 0) {
    secctionsInsideAnElement=0;
    m = -mm;
  } else {
    secctionsInsideAnElement=3;
    m = mm;
  } // end if //
    //  Dynamic allocation of global (common) arrays
    //  Dimensionamiento dinamico de arreglos globales
  dimVec(e,nmat);
  dimVec(g,nmat);
  dimVec(ct,nmat);
  dimVec(pes,nmat);
  dimMat(constraint,0,n,0,dofPerJoint);
  //      dimMat(titleCase,0,ncas,0,LineWidth);
  titleCase = new string[ncas+1];
  dimVec(loadVector,nec);
  dimVec(leftCol,nec);
  //  End of dynamic allocation of global (common) arrays

  for(auto i=1; i<= nmat; ++i) {
    posInt j;
    cin >> j;
    cin >> e[j] >> g[j] >> ct[j] >> pes [j];
  } // end for //

  titleHip  = new string [2];
  cerr << "End of Part ade1 \n";
  ade1a(n,m,nec,elementDofs,itipo,jo,totalNbrOfSprings,leftCol,e,g,reviewData,considerShearDef);
  ade1b(n,m,nec,ncas,jo,nmat,nhip,nhipr,leftCol,lin,npag,title,e,g,ct,pes, elementsBinaryFile);
  if (!reviewData) {
    ade1c(n,m,nec,elementDofs,leftCol,s);
  } // end if //
  while (icas < ncas) {
    ade1d(m,nec,elementDofs,nhip,njc,nma,nmc,icas,ncas,lin,npag,title,fileName,titleCase,ato,ct,loadVector);
    ade1e(m,elementDofs,nma,nmc,nhip,icas,ncas,lin,npag,title,fileName,titleCase,ato,ct,loadVector);    
    if (!reviewData) {
      ade1f(n,m,nec,elementDofs,jo,nhip,lin,npag,title,icas,ncas,fileName,titleCase,leftCol,loadVector,s);
    } // end if //
  } // end while //
  if (nhip > 0 and !reviewData) {
    ade1g(n,m,ncas,nhip,nhipr,lin,npag,title,fileName,titleCase, titleHip);
  } // end if //

  auto duration = std::chrono::steady_clock::now() - start;
  auto elapsed_time = std::chrono::duration_cast<ns>(duration).count();

  cout << fixed << setprecision(2)
       << "Time running " << elapsed_time*1.0e-9 
       <<  "  sec. \n";

  // removing auxiliary files
  for (int i=0; i<4; ++i) {
      remove(fileName[i]);  
  } // end for //
  // end of removing auxiliary files

  // releasing locally allocated memory
  delete[] fileName;
  freeVec(leftCol);
  freeVec(loadVector);
  delete[] titleCase;
  freeMat(constraint,0,0);  
  freeVec(pes);
  freeVec(ct);
  freeVec(g);
  freeVec(e);
  // end of releasing locally allocated memory
       
       
  file2std(cout,myStdOut);
  cout << "Bye...\n";
  return (0);
}// end of main
