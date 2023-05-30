#ifndef JOINT
#define JOINT

  #include <fstream>
  using std::fstream;

  #include <cstring>
  using std::string;

  #include "typedef.h"


  // Variables of joint header
   const size_t  dofPerJoint = 6; // nodal dof

   struct Joint {
      real x,y,z;
      real springsConstants[dofPerJoint];
      real load[dofPerJoint];
      real jointDispl[dofPerJoint];
      bool areThereSprings,areThereRestrictions;
    };

//  struct JointReactions {
//    real   reactions[dofPerJoint];
//  };

  #ifndef MAIN
    extern fstream jointsBinaryFile;
    extern Joint     jointRecord;
    extern long jointRecNber;
    extern const long sizeOfJointRecord;
    extern fstream reactionsBinaryFile;
  //  extern JointReactions jointReactionsRecord;
    extern long jointReactionRecNber;
  //  extern const long sizeOfJointReactionsRecord;

  #else
    fstream jointsBinaryFile;
    Joint     jointRecord;
    long jointRecNber;
    long sizeOfJointRecord    = sizeof(Joint);
    fstream reactionsBinaryFile;
  //  JointReactions jointReactionsRecord;
    long jointReactionRecNber;
  //  long sizeOfJointReactionsRecord    = sizeof(JointReactions);
  //  long sizeOfJointReactionsRecord;
  #endif
#endif
// end of joint.h
