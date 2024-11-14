#include "element.h"
#include <fstream>
using std::fstream;

//                            t
// for the multipication F = R * f
void mult4(const Element &member,
           const posInt  &elementDofs,
           const real *__restrict const d,
                 real *__restrict const w)
{
  for(posInt l=0; l < elementDofs; l+=3) {
    for(posInt i=1; i<=3; ++i ) {
      w[l+i] = 0.0;
      for(posInt k=i-1,j=1; j<=3; ++j,k +=3) {
        w[l+i] += d[l+j]*member.r[k];
      } // end for //
    } // end for //
  } // end for //
} // end of mult4 //
