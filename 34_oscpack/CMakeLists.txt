
cmake_minimum_required(VERSION 2.6)

ADD_DEFINITIONS(-DOSC_HOST_LITTLE_ENDIAN)

#set header & lib directories
INCLUDE_DIRECTORIES(/usr/include/oscpack)
LINK_DIRECTORIES(/usr/lib)

#build executable
ADD_EXECUTABLE(SimpleSend SimpleSend.cpp)
ADD_EXECUTABLE(SimpleReceive SimpleReceive.cpp)
ADD_EXECUTABLE(OscDump OscDump.cpp)

#link
TARGET_LINK_LIBRARIES(SimpleSend oscpack)
TARGET_LINK_LIBRARIES(SimpleReceive oscpack)
TARGET_LINK_LIBRARIES(OscDump oscpack)

