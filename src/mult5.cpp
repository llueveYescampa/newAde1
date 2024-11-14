#include <cstddef>
#include "typedef.h"
#include <numeric>
using std::inner_product;

void mult5(      real **a, 
           const real  *__restrict const b, 
                 real *__restrict const c, 
                 const posInt &rows)
{
  // for full a matrix
  for(posInt i=1; i<=rows ; ++i) {
    c[i] = inner_product( &(a[i][1]),&(a[i][rows])+1,&b[1],0.0);
  } // endfor //

  // for lower matrix
  /*
       for(posInt i=1; i<=rows ; ++i) {
       c[i] = inner_product( &(a[i][1]),&(a[i][i])+1,&b[1],0.0);
       for(posInt j=i+1; j<=rows ;++j ) {
       if (leftCol[j] <= i and a[j][i] !=0.0) {
       c[i] += a[j][i] * b[j];
       } // endif //
       } // endfor //
       } // endfor //
  */

} // end mult5() //
