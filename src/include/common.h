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
  extern real    *loadVector, **s;
  extern size_t  nec, *leftCol;
  extern size_t  n, jo, m, ncas, nhip, nhipr, itipo, npag;
  extern size_t  nmc, njc, nma,icas, lin, totalNbrOfSprings;
  extern string  title, *titleCase, *titleHip, *fileName;
  extern bool    reviewData;
  extern int     nmat;
  extern real *e, *g, *ct, *pes, ato;
  extern const size_t  elementDofs; // element dof
  extern bool    considerShearDef;
#else
         real    *loadVector, **s;
         size_t  nec, *leftCol;
         size_t  n, jo, m, ncas, nhip, nhipr, itipo, npag=0;
         size_t  nmc, njc, nma, icas=0, lin, totalNbrOfSprings=0;
         string  title,*titleCase,*titleHip, *fileName;
         bool    reviewData;
         int     nmat;
         real *e, *g, *ct, *pes, ato;
         size_t  elementDofs = 2*dofPerJoint; // element dof
         bool    considerShearDef=false;              
#endif
#endif
// end of common
