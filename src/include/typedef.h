#ifndef TYPEDEF
#define TYPEDEF
    using real = double;
    using posInt = unsigned int; 
    constexpr posInt zero = 0;
    constexpr posInt one  = 1;
    
  // Variables of joint header
   constexpr posInt dofPerJoint = 6; // nodal dof
    
   struct Joint {
      real x,y,z;
      real springsConstants[dofPerJoint];
      real load[dofPerJoint];
      real jointDispl[dofPerJoint];
      bool areThereSprings,areThereRestrictions;
    }; // end of struct Joint //

  struct Element {
    real length;
    posInt ji,jj;
    posInt iar,materialType;
    real ar,ax,ay,az,bz,by;
    real r[9];
    real stiffnessData[7];
  }; // end of struct Element //
    
#endif
