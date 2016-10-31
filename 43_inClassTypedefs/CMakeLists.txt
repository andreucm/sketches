
#to activate c++11 and pthreads
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#build
ADD_EXECUTABLE(typedefs_main main.cpp foo.cpp)

