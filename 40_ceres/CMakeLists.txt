cmake_minimum_required(VERSION 2.8)
project(ceres_sketches)

#settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

#to activate c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# external libs
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# Build
add_executable(main_jet src/main_jet.cpp)
add_executable(hello_ceres src/hello_ceres.cpp)
add_executable(sphere_fitting src/sphere_fitting.cpp)
add_executable(local_parameterization src/local_parameterization.cpp)
add_executable(camera_ext_calibration src/camera_ext_calibration.cpp)
add_executable(direction2d_parameterization src/direction2d_parameterization.cpp)

# Link
target_link_libraries(main_jet ${CERES_LIBRARIES})
target_link_libraries(hello_ceres ${CERES_LIBRARIES})
target_link_libraries(sphere_fitting ${CERES_LIBRARIES})
target_link_libraries(local_parameterization ${CERES_LIBRARIES})
target_link_libraries(camera_ext_calibration ${CERES_LIBRARIES})
target_link_libraries(direction2d_parameterization ${CERES_LIBRARIES})
