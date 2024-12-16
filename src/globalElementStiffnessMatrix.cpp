#include "prototyp.h"
#include "element.h"

void globalElementStiffnessMatrix(      real **sm, 
        const posInt  &elementDofs,
        const Element &member)
{
    real x;
    real **sa;

    dimMat(sa,elementDofs,elementDofs);

    localElementStiffnessMatrix(sm,elementDofs,member);

    for(auto l=zero; l<elementDofs ; l+=3){
        auto kkk=zero;
        for(auto kk=zero, lj=zero,j=one;j<=3;++j){
            kk = kkk++;
            lj=l+j;
            for(auto lk=zero, k=one;k<=3;++k){
                x = member.r[kk];
                kk+=3;
                if(x != 0){
                    lk = l + k;
                    for(auto i=one;i<=elementDofs;++i){
                        sa[i] [lj] += x * sm[i][ lk];
                    } // end for //
                } //end if //
            } // end for //
        } // end for //
    } // end for //

    for(auto i=one; i<=elementDofs; ++i) {
        for(auto j=one; j<=elementDofs; ++j) {
            sm[i][j] = static_cast<real>(0.0);
        } // end for //
    } // end for //

    for(auto l=zero; l<elementDofs ; l+=3){
        auto kkk=zero;
        for(auto li=zero,i=one;i<=3;++i){
            auto kk = kkk++;
            li=l+i;
            for(auto j=one;j<=elementDofs;++j){
                sm[li][j] = static_cast<real>(0.0);
            } // end for //
            for(auto lk=zero, k=one;k<=3;++k){
                x = member.r[kk];
                kk+=3;
                if(x != 0){
                    lk = l + k;
                    for(auto j=one;j<=elementDofs;++j){
                        sm[li] [j] += x * sa[lk][ j];
                    } // end for //
                } //end if //
            } // end for //
        } // end for //
    } // end for //
    freeMat(sa);
} // end of globalElementStiffnessMatrix //
