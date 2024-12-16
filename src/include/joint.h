#ifndef JOINT
    #define JOINT

    #include <fstream>
    using std::fstream;

    #include <cstring>
    using std::string;

    #include "typedef.h"

    //  struct JointReactions {
    //    real   reactions[dofPerJoint];
    //  };

    #ifndef MAIN
        extern fstream      jointsBinaryFile;
        extern Joint        jointRecord;
        extern posInt       jointRecNber;
        extern const posInt sizeOfJointRecord;
        extern fstream      reactionsBinaryFile;
        //  extern JointReactions jointReactionsRecord;
        extern posInt       jointReactionRecNber;
        //  extern const long sizeOfJointReactionsRecord;

    #else
        fstream             jointsBinaryFile;
        Joint               jointRecord;
        posInt              jointRecNber;
        posInt              sizeOfJointRecord    = sizeof(Joint);
        fstream             reactionsBinaryFile;
        //  JointReactions jointReactionsRecord;
        posInt              jointReactionRecNber;
        //  long sizeOfJointReactionsRecord    = sizeof(JointReactions);
        //  long sizeOfJointReactionsRecord;
    #endif
#endif
// end of joint.h
