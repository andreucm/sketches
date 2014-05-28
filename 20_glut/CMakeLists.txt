cmake_minimum_required(VERSION 2.6)

#Builds up all
ADD_EXECUTABLE(myGlut openGLglut.cpp)

#Link libraries
TARGET_LINK_LIBRARIES(myGlut glut GLU)
