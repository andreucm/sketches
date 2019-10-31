CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#find required packages
FIND_PACKAGE(mathgl REQUIRED)

#set header directories
INCLUDE_DIRECTORIES(${mathgl_INCLUDE_DIR})

# build and link
ADD_EXECUTABLE(mgl1 mgl1.cpp)
TARGET_LINK_LIBRARIES(mgl1 mgl-qt mgl)
