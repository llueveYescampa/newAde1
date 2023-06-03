#ifndef ELEMENT
#define ELEMENT

  #include "typedef.h"

  struct Element {
    real length;
    posInt ji,jj;
    posInt iar,materialType;
    real ar,ax,ay,az,bz,by;
    real r[9];
    real stiffnessData[7];
  };

// Variables of element header

  #ifndef MAIN
    extern fstream elementsBinaryFile;
    extern Element elementRecord;
    extern posInt elementRecNber;
    extern const posInt sizeOfElementRecord;

    extern fstream elementsForcesBinaryFile;
    extern posInt elementForcesRecNber;
    extern posInt sizeOfElementForcesRecord;

    extern posInt  secctionsInsideAnElement;

  #else

          fstream elementsBinaryFile;
          Element elementRecord;
          posInt elementRecNber;
          posInt sizeOfElementRecord  = sizeof(Element);

          fstream elementsForcesBinaryFile;
          posInt elementForcesRecNber;
          posInt sizeOfElementForcesRecord; //depens on # of sec. inside an element

          posInt  secctionsInsideAnElement=3;

  #endif
#endif
// end of element.h
