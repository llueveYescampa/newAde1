#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include <iomanip>
using std::setw;
using std::setprecision;

#define square(x) ((x)*(x))

#include <cmath>

#include "language.h"
#include "prototyp.h"
#include "joint.h"
#include "element.h"
#include "dimen.h"

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
        real  *__restrict const loadVector) 
{
    //  Local Variables
    real **sm;
    real *forcesInElement;
    real *d;
    bool logico;
    //  End of local variables



    // defining the size of the record for file elementsForcesBinaryFile
    auto totalNumberOfForces = (secctionsInsideAnElement+2)*dofPerJoint;
    dimVec(forcesInElement,0,totalNumberOfForces+1); // forcesInElement[0] takes the place of the old tlar flag
                                                     // end of defining the size of the record for file elementsForcesBinaryFile

                                                     //  Creating arrays
    dimVec(d,elementDofs);
    dimMat(sm,elementDofs,elementDofs);
    //  end of Creating arrays

    logico = true;

    if (nmc > 0) {  // Solo se ejecuta si existen miembros cargados
                    // new set of local variables
        real *wx=nullptr, *wy=nullptr, *wz=nullptr, *aw=nullptr, *bw=nullptr;
        real a,b;
        real Wx,Wy,Wz;
        real *px=nullptr, *py=nullptr, *pz=nullptr, *ap=nullptr;
        real Px,Py,Pz;
        real *wxi=nullptr, *wyi=nullptr, *wzi=nullptr,*wxj=nullptr, *wyj=nullptr, *wzj=nullptr, *awv=nullptr, *bwv=nullptr;

        real *parX=nullptr, *parY=nullptr, *parZ=nullptr, *aPar=nullptr;
        real *axialTor=nullptr, *aTor=nullptr,*bTor=nullptr;
        auto npu1=zero, npc1=zero,  npv1=zero,  npar1=zero,  ntor1=zero, ato11=zero, dty1=zero, dtz1=zero;
        real axiso,vyisoi,vyisoj,pariso, vzisoi, vzisoj;
        //auto k=zero;  // flag to define the coordinate system (global or local) of the loads
        // end of new set of local variables

        printElementInfo(2,lin);
        for(auto i=one; i<=nmc; ++i) {
            if (lin > 56) {
                header(cout,title,lin,npag);
                cout << titleCase[icas] << "\n\n";
                lin+=2;
                printElementInfo(2,lin);
            } // end if //
            cout << '\n';
            ++lin;

            for(auto i=one; i<=elementDofs; ++i) {
                d[i] = 0.0;
            } // end if //
            for(auto i=zero; i<=totalNumberOfForces; ++i) {
                forcesInElement[i] = 0.0;
            } // end if //

            // inizilizar sm, d, forcesInElement. Hace falta realmente?


            cin >> elementRecNber; --elementRecNber;// =   static_cast<long>(w[1]) -1;
            auto npu=zero, npc=zero, npv=zero, npar=zero, ntor=zero;
            real ato1, dty, dtz, by,bz;

            cin >> npu >> npc >> npv >> npar >> ntor >> ato1 >> dty >> dtz >> by >> bz;

            if (npu > 0) {
                if (wx != NULL) {
                    freeVec(wx); freeVec(wy); freeVec(wz); freeVec(aw); freeVec(bw);
                } // end if //
                dimVec(wx,npu); dimVec(wy,npu); dimVec(wz,npu); dimVec(aw,npu); dimVec(bw,npu);
            } // end if //

            if (npc > 0) {
                if (px != NULL) {
                    freeVec(px); freeVec(py); freeVec(pz); freeVec(ap);
                } // end if //
                dimVec(px,npc); dimVec(py,npc); dimVec(pz,npc); dimVec(ap,npc);
            } // end if //

            if (npv > 0) {
                if (wxi != NULL) {
                    freeVec(wxi); freeVec(wyi); freeVec(wzi);freeVec(wxj); freeVec(wyj); freeVec(wzj); freeVec(awv); freeVec(bwv);
                } // end if //
                dimVec(wxi,npv); dimVec(wyi,npv); dimVec(wzi,npv);dimVec(wxj,npv); dimVec(wyj,npv); dimVec(wzj,npv); dimVec(awv,npv); dimVec(bwv,npv);
            } // end if //

            if (npar > 0) {
                if (parX != NULL) {
                    freeVec(parX); freeVec(parY); freeVec(parZ); freeVec(aPar);
                } // end if //
                dimVec(parX,npar); dimVec(parY,npar); dimVec(parZ,npar); dimVec(aPar,npar);
            } // end if //

            if (ntor > 0) {
                if (axialTor != NULL) {
                    freeVec(axialTor); freeVec(aTor);freeVec(bTor);
                } // end if //
                dimVec(axialTor,ntor); dimVec(aTor,ntor);dimVec(bTor,ntor);
            } // end if //


            if ((npu + npc + npv + npar + ntor + ato1 + dty + dtz) == 0) {
                logico = false;
                npu = npu1;
                npc = npc1;
                npv = npv1;
                npar = npar1;
                ntor = ntor1;
                ato1 = ato11;
                dty = dty1;
                dtz = dtz1;
            } else {
                logico = true;
                npu1 = npu;
                npc1 = npc;
                npv1 = npv;
                npar1 = npar;
                ntor1 = ntor;
                ato11 = ato1;
                dty1 = dty;
                dtz1 = dtz;
            } // end if //
#ifdef ENGLISH
            cout << "Element" << setw(6) <<  elementRecNber+1
                << "         DTX =" << setw(9) << setprecision(3) << ato1
                << " DTY ="  << setw(8) << setprecision(3) << dty
                << " DTZ ="  << setw(8) << setprecision(3) << dtz << '\n';
#endif  
#ifdef ESPANOL  
            cout << "Miembro" << setw(6) <<  elementRecNber+1
                << "        ATO1 =" << setw(9) << setprecision(3) << ato1
                << " DTY="  << setw(9) << setprecision(3) << dty
                << " DTZ="   << setw(9) << setprecision(3) << dtz << '\n';
#endif  

            ++lin;
            elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
            elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);

            if (by == 0) {
                by = elementRecord.by;
            } // end if //
            if (bz == 0) {
                bz = elementRecord.bz;
            } // end if //

            axiso = 0.0;
            vyisoi = 0.0;
            vyisoj = 0.0;
            pariso = 0.0;
            vzisoi = 0.0;
            vzisoj = 0.0;


            // Calculo de las fuerzas primarias debido a cargas uniformes  // check results using https://calcresource.com/statics-fixed-beam.html
            for(auto ll = one; ll<= npu; ++ll) { // Solo se ejecuta si existen cargas uniformes
                auto k=zero;
                if (logico == false) {
                    Wx = wx[ll];
                    Wy = wy[ll];
                    Wz = wz[ll];
                    a = aw[ll];
                    b = bw[ll];
                } else {
                    cin >> k >> Wx >> Wy >> Wz >> a >> b;
                    wx[ll] = Wx;
                    wy[ll] = Wy;
                    wz[ll] = Wz;
                    aw[ll] = a;
                    bw[ll] = b;
                } // end if //
                if (a < 0.0) {
                    a = -(a*elementRecord.length);
                } // end if //
                if (b < 0.0) {
                    b = -(b*elementRecord.length);
                } else if (b == 0.0){
                    b = elementRecord.length - a;
                } // end if //


                if ( k > 0) {
                    real Wx1,Wy1,Wz1;
                    Wx1 = Wx;
                    Wy1 = Wy;
                    Wz1 = Wz;

                    if ( k == 2) {
                        jointRecNber= elementRecord.ji-1;
                        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
                        jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
                        real xi = jointRecord.x;
                        real yi = jointRecord.y;
                        real zi = jointRecord.z;

                        jointRecNber= elementRecord.jj-1;
                        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
                        jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
                        real xj = jointRecord.x;
                        real yj = jointRecord.y;
                        real zj = jointRecord.z;

                        Wx1 = Wx * sqrt( square(yj-yi) + square(zj-zi)) / elementRecord.length;
                        Wy1 = Wy * sqrt( square(xj-xi) + square(zj-zi)) / elementRecord.length;
                        Wz1 = Wz * sqrt( square(xj-xi) + square(yj-yi)) / elementRecord.length;
                    } // end if //

                    Wx = elementRecord.r[0] * Wx1 + elementRecord.r[1] * Wy1 +  elementRecord.r[2] * Wz1;
                    Wy = elementRecord.r[3] * Wx1 + elementRecord.r[4] * Wy1 +  elementRecord.r[5] * Wz1;
                    Wz = elementRecord.r[6] * Wx1 + elementRecord.r[7] * Wy1 +  elementRecord.r[8] * Wz1;
                } // end if //

                cout << "Npu = "  << setw(1) << ll
                    << "  Wx ="  << setw(9) << setprecision(3) << Wx
                    << " Wy ="  << setw(9)                    << Wy
                    << " Wz ="  << setw(9)                    << Wz
                    << " A  ="   << setw(9)                    << a
                    << " B  ="   << setw(9)                    << b << '\n';
                ++lin;

                auto myloop=zero;
                if ((elementRecord.length - a - b) > 0) {
                    myloop = 2;
                } else {
                    myloop = 1;
                } // end if //
                real ra;
                for(auto iv=one; iv<=myloop; ++iv) {
                    if (Wx != 0.0 ) {
                        d[7]  -= (elementRecord.length - a) / (elementRecord.ar * elementRecord.length) * 0.5 *Wx* (a + elementRecord.length);
                        axiso -=  Wx * (elementRecord.length - a);
                    } // end if //

                    if (Wy != 0.0 ) {
                        d[6] -=  (elementRecord.length - a) * (elementRecord.length - a) * (Wy / (24 * elementRecord.length * elementRecord.length * elementRecord.az) * (elementRecord.length * elementRecord.length + 2 * elementRecord.length * a - a * a));
                        d[12] += (Wy / (24 * elementRecord.length * elementRecord.length * elementRecord.az) * (elementRecord.length * elementRecord.length - a * a) * (elementRecord.length * elementRecord.length - a * a));
                        ra = (elementRecord.length - a) * (elementRecord.length - a) * (.5 * Wy / elementRecord.length);
                        vyisoi -=  ra;
                        vyisoj +=  (ra -  (elementRecord.length - a) * Wy );
                    } // end if //

                    if (Wz != 0.0) {
                        d[5] +=  (elementRecord.length - a) * (elementRecord.length - a) * (Wz / (24 * elementRecord.length * elementRecord.length * elementRecord.ay) * (elementRecord.length * elementRecord.length + 2 * elementRecord.length * a - a * a));
                        d[11] -= (Wz / (24 * elementRecord.length * elementRecord.length * elementRecord.ay) * (elementRecord.length * elementRecord.length - a * a) * (elementRecord.length * elementRecord.length - a * a));
                        ra = (elementRecord.length - a) * (elementRecord.length - a) * (.5 * Wz / elementRecord.length );
                        vzisoi -=  ra;
                        vzisoj +=  (ra -  (elementRecord.length - a) * Wz);
                    } // end if //

                    for(auto kk=one*12, l=one; l<=secctionsInsideAnElement; ++l) {
                        real xi = l * elementRecord.length / (secctionsInsideAnElement+1);
                        real ax,Vy,Vz,amy,amz;
                        if (xi < a) {
                            ax = 0.0;
                            Vy = 0.0;
                            Vz = 0.0;
                            amy = 0.0;
                            amz = 0.0;
                        } else {
                            ax = -(xi - a) * Wx ;
                            Vy =  (xi - a) * Wy ;
                            Vz =  (xi - a) * Wz ;
                            amy = (xi - a) * (xi - a) * (.5 * Wz );
                            amz = (xi - a) * (xi - a) * (.5 * Wy );
                        } // end if //
                        forcesInElement[++kk] += ax;
                        forcesInElement[++kk] += Vy;
                        forcesInElement[++kk] += Vz;
                        ++kk;
                        forcesInElement[++kk] += amy;
                        forcesInElement[++kk] += amz;
                    } // end for l //
                    Wx *= -1.0;
                    Wy *= -1.0;
                    Wz *= -1.0;
                    a += b;
                } // end for iv //
            } // end for //

            // Calculo de las fuerzas primarias debido a cargas concentradas  // check results using https://calcresource.com/statics-fixed-beam.html
            for(auto ll = one; ll<= npc; ++ll) { // Solo se ejecuta si existen cargas concentradas
                auto k=zero;
                if (logico == false) {
                    Px = px[ll];
                    Py = py[ll];
                    Pz = pz[ll];
                    a  = ap[ll];
                } else {
                    cin >> k >> Px >> Py >> Pz >> a;
                    px[ll] = Px;
                    py[ll] = Py;
                    pz[ll] = Pz;
                    ap[ll] = a;
                } // end if //
                if (a < 0) {
                    a = -(a*elementRecord.length);
                } // end if //

                if ( k > 0) {
                    real Px1,Py1,Pz1;
                    Px1 = Px;
                    Py1 = Py;
                    Pz1 = Pz;
                    Px = elementRecord.r[0] * Px1 + elementRecord.r[1] * Py1 +  elementRecord.r[2] * Pz1;
                    Py = elementRecord.r[3] * Px1 + elementRecord.r[4] * Py1 +  elementRecord.r[5] * Pz1;
                    Pz = elementRecord.r[6] * Px1 + elementRecord.r[7] * Py1 +  elementRecord.r[8] * Pz1;
                } // end if //


                cout << "Npc = " << setw(1) << ll
                    << "  Px =" << setw(9) << setprecision(3) << Px
                    << " Py ="  << setw(9)                    << Py
                    << " Pz ="  << setw(9)                    << Pz
                    << " A  ="  << setw(9)                    << a << '\n';
                ++lin;

                if (Px != 0.0) {
                    d[7] -=  Px * a / (elementRecord.ar * elementRecord.length);
                    axiso -=  Px;
                } // end if //

                if (Py != 0.0) {
                    d[6] -= Py * a * (elementRecord.length - a) * (2 * elementRecord.length - a) / (6 * elementRecord.length * elementRecord.length * elementRecord.az);
                    d[12] +=  Py * a * (elementRecord.length * elementRecord.length - a * a) / (6 * elementRecord.length * elementRecord.length * elementRecord.az);
                    vyisoi -=  Py * (elementRecord.length - a) / elementRecord.length;
                    vyisoj -=  Py * a / elementRecord.length;
                } // end if //

                if (Pz != 0.0) {
                    d[5] +=  Pz * a * (elementRecord.length - a) * (2 * elementRecord.length - a) / (6 * elementRecord.length * elementRecord.length * elementRecord.ay);
                    d[11] -= Pz * a * (elementRecord.length * elementRecord.length - a * a) / (6 * elementRecord.length * elementRecord.length * elementRecord.ay);
                    vzisoi -=  Pz * (elementRecord.length - a) / elementRecord.length;
                    vzisoj -=  Pz * a / elementRecord.length;
                } // end if //

                for(auto kk=one*12, l=one; l<=secctionsInsideAnElement; ++l) {
                    real xi = l * elementRecord.length / (secctionsInsideAnElement+1);
                    real ax=0.0,Vy=0.0,Vz=0.0,amy=0.0,amz=0.0;
                    if (xi >= a) {
                        ax = -Px;
                        Vy = Py;
                        Vz = Pz;
                        amy = Pz * (xi - a);
                        amz = Py * (xi - a);
                    } // end if //
                    forcesInElement[++kk] += ax;
                    forcesInElement[++kk] += Vy;
                    forcesInElement[++kk] += Vz;
                    ++kk;
                    forcesInElement[++kk] += amy;
                    forcesInElement[++kk] += amz;
                } // end for //

            } // end for //


            // Calculo de las fuerzas primarias debido a cargas linealmente variables  // check results using https://calcresource.com/statics-fixed-beam.html
            for(auto ll = one; ll<= npv; ++ll) { // Solo se ejecuta si existen linealmente variables
                auto k=zero;
                if (logico == false) {
                    Wx = wxi[ll];
                    Px = wxj[ll];
                    Wy = wyi[ll];
                    Py = wyj[ll];
                    Wz = wzi[ll];
                    Pz = wzj[ll];
                    a = awv[ll];
                    b = bwv[ll];
                } else {
                    cin >> k >> Wx >> Px >> Wy >> Py >> Wz >> Pz >> a >> b;
                    wxi[ll] = Wx;
                    wxj[ll] = Px;
                    wyi[ll] = Wy;
                    wyj[ll] = Py;
                    wzi[ll] = Wz;
                    wzj[ll] = Pz;
                    awv[ll] = a;
                    bwv[ll] = b;
                } // end if //
                if (a < 0.0) {
                    a = -(a*elementRecord.length);
                } // end if //
                if (b < 0.0) {
                    b = -(b*elementRecord.length);
                } else if (b == 0.0){
                    b = elementRecord.length-a;
                } // end if //


                if ( k > 0) {

                    real Wxi = Wx;
                    real Wyi = Wy;
                    real Wzi = Wz;

                    real Wxj = Px;
                    real Wyj = Py;
                    real Wzj = Pz;

                    if ( k == 2) {
                        jointRecNber= elementRecord.ji-1;
                        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
                        jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
                        real xi = jointRecord.x;
                        real yi = jointRecord.y;
                        real zi = jointRecord.z;

                        jointRecNber= elementRecord.jj-1;
                        jointsBinaryFile.seekg(sizeOfJointRecord*jointRecNber,ios::beg);
                        jointsBinaryFile.read(reinterpret_cast<char *> (&jointRecord), sizeOfJointRecord);
                        real xj = jointRecord.x;
                        real yj = jointRecord.y;
                        real zj = jointRecord.z;

                        real temp = sqrt( square(yj-yi) + square(zj-zi));
                        Wxi = Wx * temp / elementRecord.length;
                        Wxj = Px * temp / elementRecord.length;

                        temp = sqrt( square(xj-xi) + square(zj-zi));
                        Wyi = Wy * temp / elementRecord.length;
                        Wyj = Py * temp / elementRecord.length;

                        temp = sqrt( square(xj-xi) + square(yj-yi));
                        Wzi = Wz * temp / elementRecord.length;
                        Wzj = Pz * temp / elementRecord.length;

                    } // end if //

                    Wx = elementRecord.r[0] * Wxi + elementRecord.r[1] * Wyi +  elementRecord.r[2] * Wzi;
                    Wy = elementRecord.r[3] * Wxi + elementRecord.r[4] * Wyi +  elementRecord.r[5] * Wzi;
                    Wz = elementRecord.r[6] * Wxi + elementRecord.r[7] * Wyi +  elementRecord.r[8] * Wzi;


                    Px = elementRecord.r[0] * Wxj + elementRecord.r[1] * Wyj +  elementRecord.r[2] * Wzj;
                    Py = elementRecord.r[3] * Wxj + elementRecord.r[4] * Wyj +  elementRecord.r[5] * Wzj;
                    Pz = elementRecord.r[6] * Wxj + elementRecord.r[7] * Wyj +  elementRecord.r[8] * Wzj;

                } // end if //


                cout << "Npv = "  << setw(1) << ll
                    << "  Wxi=" << setw(9) << setprecision(3) << Wx
                    << " Wxj="  << setw(9)                    << Px
                    << " Wyi="  << setw(9)                    << Wy
                    << " Wyj="  << setw(9)                    << Py
                    << " Wzi="  << setw(9)                    << Wz
                    << " Wzj="  << setw(9)                    << Pz
                    << " A  ="   << setw(9)                    << a
                    << " B  ="   << setw(9)                    << b << '\n';
                ++lin;

                real atemp1 = Px;
                real atemp2 = Py;
                real atemp3 = Pz;
                auto myloop=zero;
                if ((elementRecord.length - a - b) > 0) {
                    Px = Wx + (Px - Wx) / b * (elementRecord.length - a);
                    Py = Wy + (Py - Wy) / b * (elementRecord.length - a);
                    Pz = Wz + (Pz - Wz) / b * (elementRecord.length - a);
                    myloop = 2;
                } else {
                    myloop = 1;
                } // end if //
                real ra;
                for(auto iv=one; iv<=myloop; ++iv) {
                    if (Wx != 0.0 or Px != 0) {
                        d[7]  -= (elementRecord.length - a) / (elementRecord.ar * elementRecord.length) * (.5 * a * (Wx + Px) + (elementRecord.length - a) * (Wx + 2 * Px) / 6);
                        axiso -=  0.5 * (Wx + Px) * (elementRecord.length - a);
                    } // end if //


                    if (Wy != 0.0 or Py != 0) {
                        d[6] -=  (elementRecord.length - a) * (elementRecord.length - a) * (Wy / (24 * elementRecord.length * elementRecord.length * elementRecord.az) * (elementRecord.length * elementRecord.length + 2 * elementRecord.length * a - a * a) + (Py - Wy) / (360 * elementRecord.length * elementRecord.length * elementRecord.az) * (7 * elementRecord.length * elementRecord.length + 6 * a * elementRecord.length - 3 * a * a));
                        d[12] += (Wy / (24 * elementRecord.length * elementRecord.length * elementRecord.az) * (elementRecord.length * elementRecord.length - a * a) * (elementRecord.length * elementRecord.length - a * a) + (Py - Wy) / (360 * elementRecord.length * elementRecord.length * elementRecord.az) * (elementRecord.length - a) * (elementRecord.length - a) * (8 * elementRecord.length * elementRecord.length + 9 * a * elementRecord.length + 3 * a * a));

                        ra = (elementRecord.length - a) * (elementRecord.length - a) * (.5 * Wy / elementRecord.length + (Py - Wy) / (6 * elementRecord.length));
                        vyisoi -=  ra;
                        vyisoj +=  (ra - 0.5 * (elementRecord.length - a) * (Wy + Py));
                    } // end if //

                    if (Wz != 0.0 or Pz != 0) {
                        d[5] +=  (elementRecord.length - a) * (elementRecord.length - a) * (Wz / (24 * elementRecord.length * elementRecord.length * elementRecord.ay) * (elementRecord.length * elementRecord.length + 2 * elementRecord.length * a - a * a) + (Pz - Wz) / (360 * elementRecord.length * elementRecord.length * elementRecord.ay) * (7 * elementRecord.length * elementRecord.length + 6 * a * elementRecord.length - 3 * a * a));
                        d[11] -= (Wz / (24 * elementRecord.length * elementRecord.length * elementRecord.ay) * (elementRecord.length * elementRecord.length - a * a) * (elementRecord.length * elementRecord.length - a * a) + (Pz - Wz) / (360 * elementRecord.length * elementRecord.length * elementRecord.ay) * (elementRecord.length - a) * (elementRecord.length - a) * (8 * elementRecord.length * elementRecord.length + 9 * a * elementRecord.length + 3 * a * a));

                        ra = (elementRecord.length - a) * (elementRecord.length - a) * (.5 * Wz / elementRecord.length + (Pz - Wz) / (6 * elementRecord.length));
                        vzisoi -=  ra;
                        vzisoj +=  (ra - .5 * (elementRecord.length - a) * (Wz + Pz));
                    } // end if //

                    for(auto kk=one*12, l=one; l<=secctionsInsideAnElement; ++l) {
                        real xi = l * elementRecord.length / (secctionsInsideAnElement+1);
                        real ax,Vy,Vz,amy,amz;
                        if (xi < a) {
                            ax = 0.0;
                            Vy = 0.0;
                            Vz = 0.0;
                            amy = 0.0;
                            amz = 0.0;
                        } else {
                            ax = -(xi - a) * (Wx + .5 * (Px - Wx) * (xi - a) / (elementRecord.length - a));
                            Vy =  (xi - a) * (Wy + .5 * (Py - Wy) * (xi - a) / (elementRecord.length - a));
                            Vz =  (xi - a) * (Wz + .5 * (Pz - Wz) * (xi - a) / (elementRecord.length - a));
                            amy = (xi - a) * (xi - a) * (.5 * Wz + (Pz - Wz) * (xi - a) / (6 * (elementRecord.length - a)));
                            amz = (xi - a) * (xi - a) * (.5 * Wy + (Py - Wy) * (xi - a) / (6 * (elementRecord.length - a)));
                        } // end if //
                        forcesInElement[++kk] += ax;
                        forcesInElement[++kk] += Vy;
                        forcesInElement[++kk] += Vz;
                        ++kk;
                        forcesInElement[++kk] += amy;
                        forcesInElement[++kk] += amz;
                    } // end for l //
                    Px = -Px;
                    Py = -Py;
                    Pz = -Pz;
                    Wx = -atemp1;
                    Wy = -atemp2;
                    Wz = -atemp3;
                    a += b;
                } // end for iv //
            } // end for ll//

            // Calculo de las fuerzas primarias debido a Momento concentrados
            for(auto ll = one; ll<= npar; ++ll) { // Solo se ejecuta si existen Momentos concentrados
                auto k=zero;
                if (logico == false) {
                    Px = parX[ll];
                    Py = parY[ll];
                    Pz = parZ[ll];
                    a  = aPar[ll];
                } else {
                    cin >> k >> Px >> Py >> Pz >> a;
                    parX[ll] = Px;
                    parY[ll] = Py;
                    parZ[ll] = Pz;
                    aPar[ll] = a;
                } // end if //
                if (a < 0) {
                    a = -(a*elementRecord.length);
                } // end if //


                if ( k > 0) {
                    real Px1,Py1,Pz1;
                    Px1 = Px;
                    Py1 = Py;
                    Pz1 = Pz;
                    Px = elementRecord.r[0] * Px1 + elementRecord.r[1] * Py1 +  elementRecord.r[2] * Pz1;
                    Py = elementRecord.r[3] * Px1 + elementRecord.r[4] * Py1 +  elementRecord.r[5] * Pz1;
                    Pz = elementRecord.r[6] * Px1 + elementRecord.r[7] * Py1 +  elementRecord.r[8] * Pz1;
                } // end if //


                cout << "Npar= " << setw(1) << ll
                    << "  Mx =" << setw(9) << setprecision(3) << Px
                    << " My ="  << setw(9)                    << Py
                    << " Mz ="  << setw(9)                    << Pz
                    << " A  ="  << setw(9)                    << a << '\n';
                ++lin;

                if (Px != 0.0) {
                    d[10]  -=  Px * a / (elementRecord.ax * elementRecord.length);
                    pariso -=  Px;
                } // end if //

                if (Py != 0.0) {
                    d[5]  -=  Py / (6 * elementRecord.ay * elementRecord.length * elementRecord.length) * (2 * elementRecord.length * elementRecord.length - 6 * a * elementRecord.length + 3 * a * a);
                    d[11] +=  Py / (6 * elementRecord.ay * elementRecord.length * elementRecord.length) * (elementRecord.length * elementRecord.length - 3 * a * a);
                    vzisoi -=  Py / elementRecord.length;
                    vzisoj +=  Py / elementRecord.length;
                } // end if //

                if (Pz != 0.0) {
                    d[6] -=  Pz / (6 * elementRecord.az * elementRecord.length * elementRecord.length) * (2 * elementRecord.length * elementRecord.length - 6 * a * elementRecord.length + 3 * a * a);
                    d[12] +=  Pz / (6 * elementRecord.az * elementRecord.length * elementRecord.length) * (elementRecord.length * elementRecord.length - 3 * a * a);
                    vyisoi +=  Pz / elementRecord.length;
                    vyisoj -=  Pz / elementRecord.length;
                } // end if //

                for(auto kk=one*16, l=one; l<=secctionsInsideAnElement; ++l) {
                    real xi = l * elementRecord.length / (secctionsInsideAnElement+1);
                    real par,amy,amz;
                    if (xi < a) {
                        par = 0.0;
                        amy = 0.0;
                        amz = 0.0;
                    } else {
                        par = -Px;
                        amy = Py;
                        amz = -Pz;
                    } // end if //
                    forcesInElement[kk] += par;
                    forcesInElement[++kk] += amy;
                    forcesInElement[++kk] += amz;
                    kk +=4;
                } // end for l//
            } // end for ll//

            // Calculo de las fuerzas primarias debido a Torsor concentrado
            for(auto ll = one; ll<= ntor; ++ll) { // Solo se ejecuta si existen Torsor concentrado
                if (logico == false) {
                    Px = axialTor[ll];
                    a = aTor[ll];
                    b = bTor[ll];
                } else {
                    cin >> Px >> a >> b;
                    axialTor[ll] = Px;
                    aTor[ll] = a;
                    bTor[ll] = b;
                } // end if //
                if (a < 0.0) {
                    a = -(a*elementRecord.length);
                } // end if //
                if (b < 0.0) {
                    b = -(b*elementRecord.length);
                } else if (b == 0){
                    b = elementRecord.length;
                } // end if //

                cout << "Ntor= " << setw(1) << ll
                    << "   T =" << setw(9) << setprecision(3) << Px
                    << " A  ="  << setw(9)                    << a
                    << " B  ="  << setw(9)                    << b << '\n';
                ++lin;
                if (Px != 0.0) {
                    d[10] -=  Px / (elementRecord.ax * elementRecord.length) * (a * b + .5 * b * b);
                    pariso -=  Px * b;
                } // end if //

                for(auto kk=one*16, l=one; l<=secctionsInsideAnElement; ++l) {
                    real xi = l * elementRecord.length / (secctionsInsideAnElement+1);
                    real par = 0.0;
                    if(xi >= a and xi <= (a + b)) {
                        par = -(xi - a) * Px;
                    } else if(xi > (a + b)) {
                        par = -Px * b;
                    } // end if //
                    forcesInElement[kk] += par;
                    kk += 6;
                } // end for l//
            } // end for ll//

            if (ato1 != 0) {
                ato1 -= ato;
                d[7] -=  ato1 * elementRecord.length * ct[elementRecord.materialType];
            } // end if //

            if (dty != 0) {
                d[6]  +=  elementRecord.length * ct[elementRecord.materialType] * dty / (2 * by);
                d[12] -=  elementRecord.length * ct[elementRecord.materialType] * dty / (2 * by);
            } // end if //

            if (dtz != 0) {
                d[5] -=  elementRecord.length * ct[elementRecord.materialType] * dtz / (2 * bz);
                d[11] +=  elementRecord.length * ct[elementRecord.materialType] * dtz / (2 * bz);
            } // end if //


            localElementStiffnessMatrix(sm, elementDofs, elementRecord);  // Calculo de la matriz de rigidez de
            mult5(sm,d,forcesInElement,elementDofs);
            //      matVectMult(sm,d,forcesInElement,elementDofs);
            forcesInElement[1] = axiso - forcesInElement[7];
            forcesInElement[2] = vyisoi + (forcesInElement[6] + forcesInElement[12]) / elementRecord.length;
            forcesInElement[3] = vzisoi - (forcesInElement[5] + forcesInElement[11]) / elementRecord.length;
            forcesInElement[4] = pariso - forcesInElement[10];
            forcesInElement[8] = vyisoj - (forcesInElement[6] + forcesInElement[12]) / elementRecord.length;
            forcesInElement[9] = vzisoj + (forcesInElement[5] + forcesInElement[11]) / elementRecord.length;


            if (nhip > 0) {
                elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
            } else {
                elementForcesRecNber=elementRecNber;
            } // end if //

            real *temp;
            dimVec(temp, 0,totalNumberOfForces+1);
            elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.read(reinterpret_cast<char *> (&temp[0]), sizeOfElementForcesRecord);


            for(auto k=one; k<=totalNumberOfForces; ++k) {
                forcesInElement[k] += temp[k];
            } // end for //
            forcesInElement[0] = 1.0;  // Flag to indicate that there is primary forces in this member
            freeVec(temp,0);
            elementsForcesBinaryFile.seekp(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
            elementsForcesBinaryFile.write(reinterpret_cast<const char *> (forcesInElement), sizeOfElementForcesRecord);
        } // end for //
        cout << '\n';
        ++lin;

        if (wx != NULL) {
            freeVec(wx); freeVec(wy); freeVec(wz); freeVec(aw); freeVec(bw);
        } // end if //
        if (px != NULL) {
            freeVec(px); freeVec(py); freeVec(pz); freeVec(ap);
        } // end if //
        if (wxi != NULL) {
            freeVec(wxi); freeVec(wyi); freeVec(wzi);freeVec(wxj); freeVec(wyj); freeVec(wzj); freeVec(awv); freeVec(bwv);
        } // end if //
        if (parX != NULL) {
            freeVec(parX); freeVec(parY); freeVec(parZ); freeVec(aPar);
        } // end if //
        if (axialTor != NULL) {
            freeVec(axialTor); freeVec(aTor);freeVec(bTor);
        } // end if //
    } // end if //



    logico = false;
    for(elementRecNber=0; elementRecNber < m; ++elementRecNber) {
        if (nhip > 0) {
            elementForcesRecNber = (icas -1) + ncas * (elementRecNber);
        } else {
            elementForcesRecNber=elementRecNber;
        } // end if //
        elementsForcesBinaryFile.seekg(sizeOfElementForcesRecord*elementForcesRecNber,ios::beg);
        elementsForcesBinaryFile.read(reinterpret_cast<char *> (forcesInElement), sizeOfElementForcesRecord);

        if (forcesInElement[0] != 0) {
            for(auto k=one; k<=elementDofs; ++k) {
                d[k] = 0.0;
            } // end for //

            elementsBinaryFile.seekg(sizeOfElementRecord*elementRecNber,ios::beg);
            elementsBinaryFile.read(reinterpret_cast< char *> (&elementRecord), sizeOfElementRecord);
            real *globalForces;
            dimVec(globalForces,elementDofs);
            mult4(elementRecord,elementDofs,forcesInElement,globalForces);
            auto k=(elementRecord.ji-1)*dofPerJoint;
            //cerr << k << '\n';
            for(auto ii=one; ii<=dofPerJoint; ++ii) {
                loadVector[++k] -= globalForces[ii];
            } // end for //
            k=(elementRecord.jj-1)*dofPerJoint;
            //cerr << k << '\n';
            for(auto ii=one; ii<=dofPerJoint; ++ii) {
                loadVector[++k] -= globalForces[ii+dofPerJoint];
            } // end for //
            freeVec(globalForces);
            if (nma + nmc + ato > 0) {
                if (lin > 53) {
                    header(cout,title,lin,npag);
                    cout << titleCase[icas] << "\n\n";
                    lin+=2;
                    printElementInfo(3,lin);
                    logico = true;
                } // end if //
                if (logico == false) {
                    printElementInfo(3,lin);
                    logico = true;
                } // end if //
                cout << setw(7) << elementRecNber+1
                    << setw(7) << elementRecord.ji << setw(16) << setprecision(3)
                    << setw(14) << -forcesInElement[1]
                    << setw(13) <<  forcesInElement[2]
                    << setw(13) <<  forcesInElement[3]
                    << setw(13) << -forcesInElement[4]
                    << setw(13) <<  forcesInElement[5]
                    << setw(13) << -forcesInElement[6] << '\n';
                cout << setw(14) <<  elementRecord.jj << setw(16)
                    << setw(14) <<  forcesInElement[7]
                    << setw(13) << -forcesInElement[8]
                    << setw(13) << -forcesInElement[9]
                    << setw(13) <<  forcesInElement[10]
                    << setw(13) << -forcesInElement[11]
                    << setw(13) <<  forcesInElement[12] << '\n';
                lin+=2;
            } // end if //
            if (lin > 55) {
                header(cout,title,lin,npag);
                cout << titleCase[icas] << "\n\n";
                lin+=2;
                printElementInfo(3,lin);
            } // end if //
        } // end if //
    } // end for //

    cout << '\n';
    ++lin;

    freeMat(sm);
    freeVec(d);
    freeVec(forcesInElement,0);

    cerr << "End of Part ade1e \n";
    // ojo puede ir a ade1f o a aed1d //
} // end of ade1e() //
