#ifndef COMMON
#define COMMON

  #include <fstream>
  using std::fstream;

  #include <cstring>
  using std::string;

  #include "typedef.h"
  #include "joint.h"

  #include "dimen.h"

  // Variables of common
  #ifndef MAIN
    extern  real    *loadVector, **s;
    extern  posInt  nec, *leftCol;
    extern  posInt  n, jo, m, ncas, nhip, nhipr, itipo, npag;
    extern  posInt  nmc, njc, nma,icas, lin, totalNbrOfSprings;
    extern  string  title, *titleCase, *titleHip, *fileName;
    extern  bool    reviewData;
    extern  int     nmat;
    extern  real *e, *g, *ct, *pes, ato;
    extern  const posInt  elementDofs; // element dof
    extern  bool    considerShearDef;
  #else
            real    *loadVector, **s;
            posInt  nec, *leftCol;
            posInt  n, jo, m, ncas, nhip, nhipr, itipo, npag=0;
            posInt  nmc, njc, nma, icas=0, lin, totalNbrOfSprings=0;
            string  title,*titleCase,*titleHip, *fileName;
            bool    reviewData;
            int     nmat;
            real *e, *g, *ct, *pes, ato;
            posInt  elementDofs = 2*dofPerJoint; // element dof
            bool    considerShearDef=false;
  #endif
#endif
// end of common
