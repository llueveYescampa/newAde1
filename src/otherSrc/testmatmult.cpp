#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::ios;

#include "typedef.h"
#include "dimen.h"    
#include "matVecMult.h"


       struct Element {
          real length;
          size_t ji,jj;
          size_t iar,materialType;
          real ar,ax,ay,az,bz,by;
          real r[9];
          real stiffnessData[7];           
       } ;



const size_t elementDofs=6;

void mult(real **sm, const Element &member)
{

  real x;
  real **sa;
  dimMat(sa,elementDofs,elementDofs);  

  
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
/*
  for (size_t i=1; i<=elementDofs; ++i) {
     for (size_t j=1; j<=elementDofs; ++j) {
        sm[i][j] = 0.0;
     } // end for //
  } // end for //
*/
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
} // end of mult //









void mult1(real **sm, real **sa, const Element &member)
{
  real x;
  
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
} // end of mult1 //

void mult2(real **sm, real **sa, const Element &member)
{
  real x;
  
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

} // end of mult2 //


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

//                            t 
// for the multipication F = R * f

void mult4(const Element &member, real *d,  real *w)
{
   for (size_t l=0; l < elementDofs; l+=3) {
      for (size_t i=1; i<=3; ++i ) {
        for (size_t k=i-1,j=1; j<=3; ++j,k +=3) {
           w[l+i] += d[l+j]*member.r[k];
        } // end for //
      } // end for //
   } // end for //
} // end of mult4 //
int main()
{  

      real **sm, **sa;
      real E=29e3,L=300,Iz=600,Ax=12;
      Element elementRecord;
      

      dimMat(sm,elementDofs,elementDofs);
      //dimMat(sa,elementDofs,elementDofs);  
      
      elementRecord.r[0] = 0.8;
      elementRecord.r[1] = 0.6;
      elementRecord.r[2] = 0.0;
      elementRecord.r[3] = -0.6;
      elementRecord.r[4] = 0.8; 
      elementRecord.r[5] = 0;
      elementRecord.r[6] = 0;
      elementRecord.r[7] = 0;
      elementRecord.r[8] = 1.0;
      
      
      sm[1][1] = E*Ax/L;
      sm[1][4] = -E*Ax/L;
      sm[4][1] = -E*Ax/L;
      sm[4][4] = E*Ax/L;
      
      sm[2][2] = 12*E*Iz/(L*L*L);
      sm[2][3] = 6*E*Iz/(L*L);
      sm[3][2] = 6*E*Iz/(L*L);
      sm[3][3] = 4*E*Iz/L;

      sm[2][5] = -12*E*Iz/(L*L*L);
      sm[2][6] = 6*E*Iz/(L*L);
      sm[3][5] = -6*E*Iz/(L*L);
      sm[3][6] = 2*E*Iz/L;

      sm[5][5] = 12*E*Iz/(L*L*L);
      sm[5][6] = -6*E*Iz/(L*L);
      sm[6][5] = -6*E*Iz/(L*L);
      sm[6][6] = 4*E*Iz/L;


      sm[5][2] = -12*E*Iz/(L*L*L);
      sm[5][3] = -6*E*Iz/(L*L);
      sm[6][2] = 6*E*Iz/(L*L);
      sm[6][3] = 2*E*Iz/L;
      
   for (size_t i=1;i<=elementDofs;++i) {    
      cout << sm[i][1] <<  "  ";
      cout << sm[i][2] <<  "  " ;
      cout << sm[i][3] <<  "  ";
      cout << sm[i][4] <<  "  ";
      cout << sm[i][5] <<  "  ";
      cout << sm[i][6] <<  "  \n";
   }    
      
   cout << "\n\n\n";
    
    //mult1(sm,sa, elementRecord);
    //mult2(sm,sa, elementRecord);
      mult(sm, elementRecord);
/*    
    real *d, *D;
    dimVec(d,elementDofs);
    dimVec(D,elementDofs);
    d[1] = 0;
    d[2] = 37.5;
    d[3] = 1875;
    d[4] = 0.0;
    d[5] = 37.5;
    d[6] = -1875;
    mult4(elementRecord,&d[0],D);
    //matVectMult(sm,D, d,elementDofs);

*/
    
    
   for (size_t i=1;i<=elementDofs;++i) {    
      cout << sm[i][1] <<  "  ";
      cout << sm[i][2] <<  "  " ;
      cout << sm[i][3] <<  "  ";
      cout << sm[i][4] <<  "  ";
      cout << sm[i][5] <<  "  ";
      cout << sm[i][6] <<  "  \n";
   }    

/*     
   for (size_t i=1;i<=elementDofs;++i) {    
      cout << D[i] <<  "  \n";
   }    
*/
  return 0;
}
