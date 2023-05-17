#include "common.h"
#include "element.h"
//                             
// for the multipication d = R * D
void mult3(const Element &member, real *d,  real *w)
{
   for (size_t l=0; l < elementDofs; l+=3) {
      for (size_t k=0, i=1; i<=3; ++i ) {
        for (size_t j=1; j<=3; ++j) {
           w[l+i] += d[l+j]*member.r[k++];
        } // end for //
      } // end for //
   } // end for //
} // end of mult3 //
