#ifndef ELEMENT
#define ELEMENT

#include "typedef.h"

       struct Element {
          real length;
          size_t ji,jj;
          size_t iar,materialType;
          real ar,ax,ay,az,bz,by;
          real r[9];
          real stiffnessData[7];           
       };

// Variables of element header

#ifndef MAIN
  extern fstream elementsBinaryFile;
  extern Element elementRecord;
  extern long elementRecNber;
  extern const long sizeOfElementRecord;

  extern fstream elementsForcesBinaryFile;  
  extern long elementForcesRecNber;
  extern       long sizeOfElementForcesRecord;

  extern size_t  secctionsInsideAnElement;

#else
  
         fstream elementsBinaryFile;
         Element elementRecord;
         long elementRecNber;
         long sizeOfElementRecord  = sizeof(Element);

         fstream elementsForcesBinaryFile;
         long elementForcesRecNber;
         long sizeOfElementForcesRecord; //depens on # of sec. inside an element

         size_t  secctionsInsideAnElement=3;

#endif
#endif
// end of element.h
