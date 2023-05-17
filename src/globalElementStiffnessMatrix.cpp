#include "common.h"
#include "prototyp.h"
#include "element.h"

void globalElementStiffnessMatrix(real **sm, const Element &member)
{
  real x;
  real **sa;
  dimMat(sa,elementDofs,elementDofs);  

  localElementStiffnessMatrix(sm, member);
  
  for(size_t kkk, l=0; l<elementDofs ; l+=3){
     kkk=0;
     for(size_t kk, lj,j=1;j<=3;++j){
       kk = kkk++;
       lj=l+j;
       for(size_t lk, k=1;k<=3;++k){
          x = member.r[kk];
          kk+=3;
          if(x != 0){
            lk = l + k;
            for(size_t i=1;i<=elementDofs;++i){
               sa[i] [lj] += x * sm[i][ lk];
            } // end for //
          } //end if //
       } // end for //  
     } // end for //
  } // end for //

  for (size_t i=1; i<=elementDofs; ++i) {
     for (size_t j=1; j<=elementDofs; ++j) {
        sm[i][j] = 0.0;
     } // end for //
  } // end for //

  for(size_t kkk, l=0; l<elementDofs ; l+=3){
     kkk=0;
     for(size_t kk, li,i=1;i<=3;++i){
       kk = kkk++;
       li=l+i;
       for(size_t j=1;j<=elementDofs;++j){
         sm[li][j] = 0.0;
       } // end for //
       for(size_t lk, k=1;k<=3;++k){
          x = member.r[kk];
          kk+=3;
          if(x != 0){
            lk = l + k;
            for(size_t j=1;j<=elementDofs;++j){
               sm[li] [j] += x * sa[lk][ j];
            } // end for //
          } //end if //
       } // end for //  
     } // end for //
  } // end for //
  freeMat(sa);
} // end of globalElementStiffnessMatrix //
