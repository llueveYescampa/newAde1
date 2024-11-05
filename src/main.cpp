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

#include "newade1.h"
#include "prototyp.h"

int main(int argc, char *argv[])
{

  //  Local Variables
  clock_t start;

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
    std2file(cin,infile);
    if (argc >= 3) {
      outfile.open(argv[2], ios::out);
      std2file(cout,outfile);
    } // endif //
  } // endif //
  path p = argv[1];
    //    cin.clear();

    // assigning file names
  fileName = new string[4];
  //fileName[0] = strtok(argv[1],".");
  fileName[0] = p.replace_extension();
  fileName[1] = fileName[0] + ".elm";
  fileName[2] = fileName[0] + ".efs";
  fileName[3] = fileName[0] + ".rfs";
  fileName[0] += ".jnt";
  
  cout << fileName[0] << '\n';
  cout << fileName[1] << '\n';
  cout << fileName[2] << '\n';
  cout << fileName[3] << '\n'; //exit(0);
  
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
  header(cerr);
  start = clock();

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

  ade1a();
  ade1b();
  if (!reviewData) {
    ade1c();
  } // end if //
  while (icas < ncas) {
    ade1d();
    ade1e();
    if (!reviewData) {
      ade1f();
    } // end if //
  } // end while //

  if (nhip > 0 and !reviewData) {
    ade1g();
  } // end if //

    //freeVec(w);
  freeMat(constraint,0,0);
  freeVec(e);
  freeVec(g);
  freeVec(ct);
  freeVec(pes);
  
  // removing auxiliary files
  for (int i=0; i<4; ++i) {
      remove(fileName[i]);  
  } // end for //
  // end of removing auxiliary files

  cout << fixed << setprecision(2)
       << "Time running " << static_cast<double>(clock() - start) / CLOCKS_PER_SEC
       <<  "  sec. \n";
  return (0);
}// end of main
