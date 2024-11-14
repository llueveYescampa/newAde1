#include "element.h"

void localElementStiffnessMatrix(      real **sm, 
                                 const posInt  &elementDofs,
                                 const Element &member)
{

    real al=member.length;
    real el=member.ar;
    real gkl=member.ax;
    real eiyl=member.ay;
    real eizl=member.az;

    real ciy = member.stiffnessData[0];
    real cjy = member.stiffnessData[1];
    real cy  = member.stiffnessData[2];
    real c   = member.stiffnessData[3];
    real ciz = member.stiffnessData[4];
    real cjz = member.stiffnessData[5];
    real cz  = member.stiffnessData[6];

    real ct1, ct2,ct3;
    //real el2,el3;
    real eiyl2, eiyl3, eizl2, eizl3;

    for(auto i=one; i<=elementDofs ; ++i) {
       for(auto j=one; j<=i ; ++j) {
          sm[i][j] = static_cast<real>(0.0);
        } // end for //
     } // end for //

    //el2 = el / al;
    //el3 = el2 / al;
    eiyl2 = eiyl / al;
    eiyl3 = eiyl2 / al;
    eizl2 = eizl / al;
    eizl3 = eizl2 / al;

    sm[1][ 1]   =   el * c;
    sm[7][ 1]   =  -el * c;
    sm[7][ 7]   =   el * c;
    sm[4][ 4]   =  gkl * c;
    sm[10][4]   = -gkl * c;
    sm[10][ 10] =  gkl * c;

    ct1 = ciz + cz;
    ct2 = cjz + cz;
    ct3 = ct1 + ct2;

    sm[2][ 2] =  eizl3 * ct3;
    sm[6][ 2] =  eizl2 * ct1;
    sm[8][ 2] = -eizl3 * ct3;
    sm[12][ 2] = eizl2 * ct2;

    sm[6][ 6] =   eizl  * ciz;
    sm[8][ 6] =  -eizl2 * ct1;
    sm[12][6] =   eizl  * cz;
    sm[8][ 8] =   eizl3 * ct3;
    sm[12][8] =  -eizl2 * ct2;
    sm[12][ 12] = eizl  * cjz;

    ct1 = ciy + cy;
    ct2 = cjy + cy;
    ct3 = ct1 + ct2;

    sm[3][ 3] =  eiyl3 * ct3;
    sm[5][3] =  -eiyl2 * ct1;
    sm[9][3] =  -eiyl3 * ct3;
    sm[11][3] = -eiyl2 * ct2;

    sm[5][ 5] =   eiyl * ciy;
    sm[9][5] =   eiyl2 * ct1;
    sm[11][5] =   eiyl * cy;
    sm[9][ 9] =  eiyl3 * ct3;
    sm[11][9] =  eiyl2 * ct2;
    sm[11][ 11] = eiyl * cjy;

    for(auto i=one; i<=elementDofs; ++i) {
      for(auto j=i+1; j<=elementDofs; ++j) {
        sm[i][j]= sm[j][i];
      } // end for //
    } // end for //

} // end of localElementStiffnessMatrix //
