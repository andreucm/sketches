cmake_minimum_required (VERSION 2.6)
project (threads_tests)

#to activate c++11 and pthreads
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#find threads lib
# find_package (Threads)

# build tests
ADD_EXECUTABLE(threads_1 threads_1.cpp)
# TARGET_LINK_LIBRARIES(threads_1 ${CMAKE_THREAD_LIBS_INIT})
