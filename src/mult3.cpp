#include "element.h"
//
// for the multipication d = R * D
void mult3(const Element &member, 
        const posInt  &elementDofs,
        const real *__restrict const d,
        real *__restrict const w)
{
    for(posInt l=0; l < elementDofs; l+=3) {
        for(posInt k=0, i=1; i<=3; ++i ) {
            for(posInt j=1; j<=3; ++j) {
                w[l+i] += d[l+j]*member.r[k++];
            } // end for //
        } // end for //
    } // end for //
} // end of //mult3 //
