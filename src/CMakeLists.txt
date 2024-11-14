# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
#Create a variable called linalg_SOURCES containing all .cpp files:


set(newAde1_Src
    src/ade1a.cpp
    src/ade1a.cpp
    src/ade1c.cpp
    src/ade1e.cpp  
    src/ade1g.cpp
    src/header.cpp
    src/main.cpp   
    src/mult4.cpp  
    src/printElementInfo.cpp  
    src/ade1b.cpp
    src/ade1d.cpp  
    src/ade1f.cpp  
    src/globalElementStiffnessMatrix.cpp  
    src/localElementStiffnessMatrix.cpp  
    src/mult3.cpp  
    src/mult5.cpp  
    src/printJointInfo.cpp    
    PARENT_SCOPE
   )
