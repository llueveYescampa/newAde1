#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <fstream>
using std::ifstream;

#include<cstdlib>

#include <cmath>

#include <iomanip>
using std::setw;
using std::fixed;
using std::setprecision;

#include "language.h"
#include "constraint.h"
#include "joint.h"
#include "element.h"

#include "typedef.h"
#include "dimen.h"

inline real sqrval(real s) {return (s) * (s);}
inline real cube(real s) {return (s)*(s)*(s);}

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
        bool &considerShearDef ) 
{

    //  Local Variables
    posInt *gdof;
    real z1,z2; // segmentos de rigidez infinita
    bool isThereAPointP;
    real xp,yp,zp;
    real ara=0.0,axa=0.0,aya=0.0,aza=0.0,bza=0.0,bya=0.0;
    real fy,fz,fya=0.0,fza=0.0;
    //  End of local variables


    //  Dynamic allocation of local arrays
    dimVec(gdof,elementDofs);
    //  End of dynamic allocation of local arrays

    for(auto i=one; i<=nec ;++i ) { // inizializing left column //
        leftCol[i] = i;
    } // endfor //

    for(auto i=zero; i<dofPerJoint ;++i ) { // inizializing the joint load //
        jointRecord.load[i]  = 0.0;
        jointRecord.jointDispl[i] = 0.0;
    } // endfor //


    // This correction is done just all the joints.
    for(auto i=zero;i<n;++i) {
        if (itipo == 1) {
            constraint[i][2]=true;
            constraint[i][3]=true;
            constraint[i][4]=true;
        } else if(itipo == 2){
            constraint[i][0]=true;
            constraint[i][1]=true;
            constraint[i][5]=true;
        } // end if //
    } // end for //

    for(auto i=one; i<=n ; ++i) {
        cin >> jointRecNber >> jointRecord.x >> jointRecord.y >> jointRecord.z;
        --jointRecNber;
        jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
        jointsBinaryFile.write(reinterpret_cast<const char *> (&jointRecord), sizeOfJointRecord);
    } // end for //


    for(auto i=one;i<=jo;++i) {
        jointRecord.areThereSprings=false;
        jointRecord.areThereRestrictions=false;
        cin >> jointRecNber;
        --jointRecNber;
        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
        jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);
        for(auto k = zero; k < dofPerJoint; ++k){
            cin >> constraint[jointRecNber][k];
        } // end for//

        // This correction is done just for the constraint joins.
        if (itipo == 1) {
            // For itipo=1 the constraint dof are 3,4, and 5 which are in positions 2,3,and 4 of the constraint array
            constraint[jointRecNber][2]=true;
            constraint[jointRecNber][3]=true;
            constraint[jointRecNber][4]=true;
        } else if (itipo == 2) {
            // For itipo=2 the constraint dof are 1,2, and 6 which are in positions 0,1,and 5 of the constraint array
            constraint[jointRecNber][0]=true;
            constraint[jointRecNber][1]=true;
            constraint[jointRecNber][5]=true;
        } // end if

        for(auto k = zero; k < dofPerJoint; ++k) {
            cin >> jointRecord.springsConstants[k];
            if (jointRecord.springsConstants[k] > 0.0) {
                jointRecord.areThereSprings=true;
                jointRecord.areThereRestrictions=true;
                ++totalNbrOfSprings;  // now I am thinking this is not necesary.....
            } else if (constraint[jointRecNber][k]) {
                jointRecord.areThereRestrictions=true;
            } // end if //
        } // end for//
        jointsBinaryFile.seekp(sizeOfJointRecord*jointRecNber,ios::beg);
        jointsBinaryFile.write(reinterpret_cast< const char *> (&jointRecord), sizeOfJointRecord);
    } // end for//

    for(auto j=one; j<=m; ++j) {
        int ii, ji,jj;
        cin >> ii >> ji >> jj >> elementRecord.materialType
            >> elementRecord.ar >> elementRecord.ax >> elementRecord.ay >> elementRecord.az
            >> z1 >> z2;

        if (ji < 0 and jj > 0) {
            elementRecord.iar = 1;
        } else if (ji > 0 and jj < 0) {
            elementRecord.iar = 2;
        } else if (ji < 0 and jj < 0) {
            elementRecord.iar = 3;
        } else {
            elementRecord.iar = 0;
        } // end if //


        elementRecord.ji =  abs(ji);
        elementRecord.jj =  abs(jj);
        //       Calculo de leftColumn
        for(auto k=one; k <= dofPerJoint; ++k) {
            gdof[k] = (elementRecord.ji-1)*dofPerJoint + k;
            if (constraint[elementRecord.ji-1][k-1] == true) {
                gdof[k] = 0;
            } // end if
        } // endfor //

        for(auto k=dofPerJoint+1; k <= elementDofs; ++k) {
            gdof[k]= (elementRecord.jj-2)*dofPerJoint + k;
            if (constraint[elementRecord.jj-1][k-dofPerJoint-1] == true) {
                gdof[k] = 0;
            } // end if
        } // endfor //

        if (elementRecord.iar==3 and itipo == 1) { // this is a 2d Truss element
            gdof[dofPerJoint] = 0;
            gdof[elementDofs] = 0;
        } // end if //

        for(auto k=one; k <= elementDofs; ++k) {
            if(gdof[k] > 0) {
                for(auto l=k+1; l <= elementDofs; ++l) {
                    if(gdof[l] > 0) {
                        auto row = gdof[l];
                        auto col = gdof[k];

                        //if ( gdof[k] > gdof[l]  ) {
                        if ( col > row  ) {
                            row = gdof[k];
                            col = gdof[l];
                        } // endif //
                        if ( col < leftCol[row] ) {
                            leftCol[row]= col;
                        } // endif //
                    } // endif //
                    } // endfor //
                } // end if //
            } // endfor //
              //       End of Calculo de leftColumn

            if (elementRecord.materialType == 0) {
                elementRecord.materialType = 1;
            } // end if //

            if ( ii < 0) {   // negative ii mean : there is a P point
                elementRecNber = -(ii+1);
                isThereAPointP=true;
                cin >> xp >> yp >> zp;
            } else {
                elementRecNber = ii-1;
                isThereAPointP=false;
            } // end if //


            // como quedan aqui z1 and z2 ?
            if ((elementRecord.ar + elementRecord.ax + elementRecord.ay + elementRecord.az) == 0) {
                elementRecord.ar = ara;
                elementRecord.ax = axa;
                elementRecord.ay = aya;
                elementRecord.az = aza;
                elementRecord.bz = bza;
                elementRecord.by = bya;
                fy = fya;
                fz = fza;
            } else {
                if ((elementRecord.ay + elementRecord.az) != 0) {
                    elementRecord.bz = 0.0;
                    elementRecord.by = 0.0;
                    fy = elementRecord.ay * 1.5 / elementRecord.ar;
                    fz = elementRecord.az * 1.5 / elementRecord.ar;
                } else { // elementRecord.ay + elementRecord.az == 0)
                    elementRecord.bz = elementRecord.ar;
                    elementRecord.by = elementRecord.ax;
                    if (elementRecord.ax == 0) { // Seccion circular
                        real d = 0.5 * elementRecord.bz;
                        elementRecord.ar = M_PI * sqrval(d);
                        elementRecord.ax = 0.5 * elementRecord.ar * sqrval(d);
                        elementRecord.ay = elementRecord.az = 0.5 * elementRecord.ax;
                        fy = elementRecord.ay * 10 / (elementRecord.ar * 9);
                        fz = elementRecord.az * 10 / (elementRecord.ar * 9);
                    } else { // Seccion rectangular
                        elementRecord.by = elementRecord.ax;
                        elementRecord.ar *=  elementRecord.ax;
                        elementRecord.ax = elementRecord.ar / 12;
                        elementRecord.ay = elementRecord.ax * sqrval(elementRecord.bz);
                        elementRecord.az = elementRecord.ax * sqrval(elementRecord.by);
                        real c,d;
                        if (elementRecord.bz < elementRecord.by) {
                            c = elementRecord.bz;
                            d = elementRecord.by;
                        } else {
                            c = elementRecord.by;
                            d = elementRecord.bz;
                        } // end if //
                        real f = c / d;
                        elementRecord.ax = cube(c) * d * (1 - .63 * f + .0525 * sqrval(f)*sqrval(f)*f) / 3;
                        fy = elementRecord.ay * 1.2 / elementRecord.ar;
                        fz = elementRecord.az * 1.2 / elementRecord.ar;
                    } // end if //
                } // end if //
                ara = elementRecord.ar;
                axa = elementRecord.ax;
                aya = elementRecord.ay;
                aza = elementRecord.az;
                bza = elementRecord.bz;
                bya = elementRecord.by;
                fya = fy;
                fza = fz;
            } // end if //



            if (elementRecord.ji >= elementRecord.jj) {
#ifdef ENGLISH
                cout << "ERROR JI >= JJ IN ELEMENT: " << setw(6) << elementRecNber+1 << '\n';
                cerr << "ERROR JI >= JJ IN ELEMENT: " << setw(6) << elementRecNber+1 << '\n';
#endif  
#ifdef ESPANOL  
                cout << "ERROR JI >= JJ EN EL MIEMBRO:" << setw(6) << elementRecNber+1 << '\n';
                cerr << "ERROR JI >= JJ EN EL MIEMBRO:" << setw(6) << elementRecNber+1 << '\n';
#endif  
                reviewData = true; // from now on only check input data
            } // end if //

            //jointRecNber=elementRecord.ji;
            jointsBinaryFile.seekg(sizeOfJointRecord*(elementRecord.ji-1));
            jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);
            real xlp = jointRecord.x;
            real ylp = jointRecord.y;
            real zlp = jointRecord.z;


            //jointRecNber=elementRecord.jj;
            jointsBinaryFile.seekg(sizeOfJointRecord*(elementRecord.jj-1));
            jointsBinaryFile.read(reinterpret_cast< char *> (&jointRecord), sizeOfJointRecord);
            real cx = jointRecord.x;
            real cy = jointRecord.y;
            real cz = jointRecord.z;

            cx -= xlp;
            cy -= ylp;
            cz -= zlp;

            elementRecord.length = sqrt(cx * cx + cy * cy + cz * cz);
            cx /= elementRecord.length;
            cy /= elementRecord.length;
            cz /= elementRecord.length;
            if (isThereAPointP) {
                xlp = xp - xlp;
                ylp = yp - ylp;
                zlp = zp - zlp;
            } // end if //
            real q = (cx * cx) + (cz * cz);

            if (q < .001) {  // Vertical members
                for(auto k=zero; k<9;++k){
                    elementRecord.r[k] = 0.0;
                } // end for//
                elementRecord.r[1] = cy;
                if (isThereAPointP==false) { // Miembro vertical sin rotacion longitudinal
                    elementRecord.r[3] = -cy;
                    elementRecord.r[8] = 1.0;
                } else {                     //Miembro vertical con rotacion longitudinal
                    real a = sqrt(xlp * xlp + zlp * zlp);
                    real c = -xlp * cy / a;
                    real ss = zlp / a;
                    elementRecord.r[3] = -c * cy;
                    elementRecord.r[5] = ss;
                    elementRecord.r[6] = ss * cy;
                    elementRecord.r[8] = c;
                } // end if //
            } else { // No vertical members
                q = sqrt(q);
                elementRecord.r[0] = cx;
                elementRecord.r[1] = cy;
                elementRecord.r[2] = cz;
                // Miembros no verticales sin rotacion longitudinal
                elementRecord.r[3] = -cx * cy / q;
                elementRecord.r[4] = q;
                elementRecord.r[5] = -cy * cz / q;
                elementRecord.r[6] = -cz / q;
                elementRecord.r[8] = cx / q;
                if (isThereAPointP==false) {
                    elementRecord.r[7] = 0;
                } else { // miembro no vertical con rotacion longitudinal
                    real a = elementRecord.r[3] * xlp + elementRecord.r[4] * ylp + elementRecord.r[5] * zlp;
                    real c = elementRecord.r[6] * xlp + elementRecord.r[8] * zlp;
                    real d = sqrt(a * a + c * c);
                    real ss = c / d;
                    c = a / d;
                    // cerr << " cos and sine of alpha : "<< c << "   " << ss << "\n";
                    elementRecord.r[3] = (-cx * cy * c - cz * ss) / q;
                    elementRecord.r[4] = q * c;
                    elementRecord.r[5] = (-cz * cy * c + cx * ss) / q;
                    elementRecord.r[6] = (cx * cy * ss - cz * c) / q;
                    elementRecord.r[7] = -q * ss;
                    elementRecord.r[8] = (cy * cz * ss + cx * c) / q;
                } // end if //
            } // end if //

            z1 /= elementRecord.length;
            z2 /= elementRecord.length;
            real z3 = 1 - z1 - z2;


            if (considerShearDef) {
                fy *= e[elementRecord.materialType] / (g[elementRecord.materialType] * sqrval(elementRecord.length));
                fz *= e[elementRecord.materialType] / (g[elementRecord.materialType] * sqrval(elementRecord.length));
            } else {
                fy = 0;
                fz = 0;
            } // end if //

            real c1 = z3 * (3 * z1 * z3 + 3 * z1 * z1 + z3 * z3) / 3;
            real c2 = z3 * (3 * z2 * z3 + 3 * z2 * z2 + z3 * z3) / 3;
            real c3 = z3 * (6 * z2 * z1 + 3 * z3 - 2 * z3 * z3)  / 6;

            elementRecord.stiffnessData[0] = c1 + fy * z3;
            elementRecord.stiffnessData[1] = c2 + fy * z3;
            elementRecord.stiffnessData[2] = c3 - fy * z3;
            elementRecord.stiffnessData[3] = elementRecord.stiffnessData[0] * elementRecord.stiffnessData[1] - sqrval(elementRecord.stiffnessData[2]);

            elementRecord.stiffnessData[0] /=  elementRecord.stiffnessData[3];
            elementRecord.stiffnessData[1] /=  elementRecord.stiffnessData[3];
            elementRecord.stiffnessData[2] /=  elementRecord.stiffnessData[3];

            elementRecord.stiffnessData[4] = c1 + fz * z3;
            elementRecord.stiffnessData[5] = c2 + fz * z3;
            elementRecord.stiffnessData[6] = c3 - fz * z3;

            elementRecord.stiffnessData[3] = elementRecord.stiffnessData[4] * elementRecord.stiffnessData[5] - sqrval(elementRecord.stiffnessData[6]);

            elementRecord.stiffnessData[4] /=  elementRecord.stiffnessData[3];
            elementRecord.stiffnessData[5] /=  elementRecord.stiffnessData[3];
            elementRecord.stiffnessData[6] /=  elementRecord.stiffnessData[3];

            elementRecord.stiffnessData[3] = 1 / z3;

            if (elementRecord.iar == 1) {
                elementRecord.stiffnessData[5] -= sqrval(elementRecord.stiffnessData[6])/ elementRecord.stiffnessData[4];
                elementRecord.stiffnessData[4] = 0;
                elementRecord.stiffnessData[6] = 0;
                elementRecord.stiffnessData[1] -= sqrval(elementRecord.stiffnessData[2]) / elementRecord.stiffnessData[0];
                elementRecord.stiffnessData[0] = 0;
                elementRecord.stiffnessData[2] = 0;
            } else if (elementRecord.iar == 2) {
                elementRecord.stiffnessData[4] -=  sqrval(elementRecord.stiffnessData[6]) / elementRecord.stiffnessData[5];
                elementRecord.stiffnessData[5] = 0;
                elementRecord.stiffnessData[6] = 0;
                elementRecord.stiffnessData[0] -=  sqrval(elementRecord.stiffnessData[2])  / elementRecord.stiffnessData[1];
                elementRecord.stiffnessData[1] = 0;
                elementRecord.stiffnessData[2] = 0;
            } else if (elementRecord.iar == 3) {
                elementRecord.stiffnessData[0] = 0;
                elementRecord.stiffnessData[1] = 0;
                elementRecord.stiffnessData[2] = 0;
                elementRecord.stiffnessData[4] = 0;
                elementRecord.stiffnessData[5] = 0;
                elementRecord.stiffnessData[6] = 0;
                elementRecord.ax = 0.0;
            } // end if //

            real a = e[elementRecord.materialType] / elementRecord.length;
            elementRecord.ar *= a;
            elementRecord.ay *= a;
            elementRecord.az *= a;
            elementRecord.ax *= (g[elementRecord.materialType] / elementRecord.length);


            elementsBinaryFile.seekp((elementRecNber++)*sizeOfElementRecord);
            elementsBinaryFile.write(reinterpret_cast< const char *> (&elementRecord), sizeOfElementRecord);


            cerr << "Mbro= " <<  setw(3) << elementRecNber
                <<  setw(12) <<  "Ji="  <<  setw(3) << ji
                <<  setw(12) <<  "Jj="  <<  setw(3) << jj
                <<  setw(12) <<  "L=  " <<  setw(3) << fixed << setprecision(3) << elementRecord.length << '\n';
        } // end for//


        freeVec(gdof);

        cerr << "End of Part ade1a \n";
    } // end of ade1a //
