#ifndef PROTOTYP_H
    #define PROTOTYP_H

    #include <fstream>
    using std::ostream;
    using std::fstream;

    #include <cstring>
    using std::string;


    #include "element.h"


    void ade1a(const posInt  &n,
            const posInt  &m,
            const posInt  &nec, 
            const posInt  &elementDofs,
            const posInt  &itipo,
            const posInt  &jo,
            posInt  &totalNbrOfSprings,
            posInt  *__restrict const leftCol, 
            const real  *__restrict const e,
            const real  *__restrict const g,             
            bool &reviewData,
            bool &considerShearDef);             

    void ade1b(const posInt  &n,
            const posInt  &m,
            const posInt  &nec, 
            const posInt  &ncas, 
            const posInt  &jo,
            const posInt  &nmat, 
            const posInt  &nhip, 
            const posInt  &nhipr,
            const posInt  *__restrict const leftCol, 
            posInt  &lin,
            posInt  &npag,
            const string title,      
            const real  *__restrict const e,
            const real  *__restrict const g,
            const real  *__restrict const ct,
            const real  *__restrict const pes,
            fstream &elementsBinaryFile);

    void ade1c(const posInt  &n,
            const posInt  &m,
            const posInt  &nec,
            const posInt  &elementDofs,
            posInt  *leftCol,
            real    **(&s));

    void ade1d(const posInt  &m,  
            const posInt  &nec, 
            const posInt  &elementDofs,             
            const posInt  &nhip,
            posInt  &njc,
            posInt  &nma,
            posInt  &nmc,
            posInt  &icas,
            posInt  &ncas,
            posInt  &lin,
            posInt  &npag,
            const string title,      
            string *fileName,
            string *titleCase,
            real    &ato,
            const real  *__restrict const ct,
            real  *__restrict const loadVector);


    void ade1e(const posInt  &m,  
            const posInt  &elementDofs,  
            const posInt  &nma,
            const posInt  &nmc,
            const posInt  &nhip,
            const posInt  &icas,
            const posInt  &ncas,
            posInt  &lin,
            posInt  &npag,
            const string title,
            const string *fileName,
            const string *titleCase,
            const real    &ato,
            const real  *__restrict const ct,
            real  *__restrict const loadVector);  

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
            real    **(&s));

    void ade1g(const posInt  &n,
            const posInt  &m,
            const posInt  &ncas,
            const posInt  &nhip,
            posInt  &nhipr,             
            posInt  &lin,
            posInt  &npag,
            const string title,      
            const string *fileName,
            const string *titleCase,
            string *titleHip);


    void header(ostream &mycout,
            string title,
            posInt &lin,
            posInt &npag);

    void printElementInfo(const posInt &val, posInt &lin);
    void printJointInfo  (const posInt &val, posInt &lin);
    void localElementStiffnessMatrix(real **sm, const posInt  &elementDofs, const Element &member);
    void globalElementStiffnessMatrix(real **sm, const posInt  &elementDofs, const Element &member);

    //void mult1(real **sm, const Element &member,  real **sa);
    //void mult2(const Element &member, real **sa, real **sm);
    void mult3(const Element &member, const posInt  &elementDofs, const real *__restrict const d,  real *__restrict const w);
    void mult4(const Element &member, const posInt  &elementDofs, const real *__restrict const d,  real *__restrict const w);
    void mult5(real **sm, const real *__restrict const d,  real *__restrict const w, const posInt &rows);

#endif // End of prototyp_h
