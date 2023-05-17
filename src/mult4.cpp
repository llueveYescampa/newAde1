#include "common.h"
#include "element.h"
//                            t 
// for the multipication F = R * f
void mult4(const Element &member, real *d,  real *w)
{
   for (size_t l=0; l < elementDofs; l+=3) {
      for (size_t i=1; i<=3; ++i ) {
        w[l+i] = 0.0;
        for (size_t k=i-1,j=1; j<=3; ++j,k +=3) {
           w[l+i] += d[l+j]*member.r[k];
        } // end for //
      } // end for //
   } // end for //
} // end of mult4 //
