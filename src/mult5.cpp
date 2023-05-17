#include <cstddef>
#include "typedef.h"
#include <numeric>
using std::inner_product;

void mult5(real **a, const real  *b, real *c, const size_t &rows)
{
// for full a matrix   
   for (size_t i=1; i<=rows ; ++i) {
      c[i] = inner_product( &(a[i][1]),&(a[i][rows])+1,&b[1],0.0);
   } // endfor //

// for lower matrix
/*   
   for (size_t i=1; i<=rows ; ++i) {
      c[i] = inner_product( &(a[i][1]),&(a[i][i])+1,&b[1],0.0);
      for (size_t j=i+1; j<=rows ;++j ) {
         if (leftCol[j] <= i and a[j][i] !=0.0) {
            c[i] += a[j][i] * b[j];
         } // endif //
      } // endfor //
   } // endfor //
*/   
   
} // end mult5() //

