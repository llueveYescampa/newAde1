#ifndef ELEMENT
    #define ELEMENT

    #include "typedef.h"
    #include "dimen.h"
    #include <fstream>
    using std::fstream;


    // Variables of element header

    #ifndef MAIN
        extern Element elementRecord;

        extern fstream elementsBinaryFile;
        extern posInt elementRecNber;
        extern const posInt sizeOfElementRecord;
        extern fstream elementsForcesBinaryFile;
        extern posInt elementForcesRecNber;
        extern posInt sizeOfElementForcesRecord;
        extern posInt  secctionsInsideAnElement;

    #else

        Element elementRecord;

        fstream elementsBinaryFile;
        posInt elementRecNber;
        posInt sizeOfElementRecord  = sizeof(Element);
        fstream elementsForcesBinaryFile;
        posInt elementForcesRecNber;
        posInt sizeOfElementForcesRecord; //depens on # of sec. inside an element
        posInt  secctionsInsideAnElement=3;

    #endif
#endif
// end of element.h
