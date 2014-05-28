cmake_minimum_required(VERSION 2.6)

# add the necessary include directories
SET(dep_INCLUDE_DIR
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/3d"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/app"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/communication"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/events"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/gl"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/graphics"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/math"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/sound"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/types"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/utils"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/openFrameworks/video"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/tess2/include/"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/poco/include/"
      "/home/andreu/dev/extSw/of_v0.8.0_linux64_release/libs/kiss/include/"
      "/usr/include/cairo/"
      "/usr/include/gstreamer-0.10/"
      "/usr/include/glib-2.0/"
      "/usr/lib/x86_64-linux-gnu/glib-2.0/include/"
)

SET(dep_LIBRARY

)

# application source files
SET(SRCS
      testApp.cpp
)

SET(HEADERS 
      testApp.h
)

#required include directories
INCLUDE_DIRECTORIES(${dep_INCLUDE_DIR})

# Build an executable
ADD_EXECUTABLE(ofxTest main.cpp)

# link necessary libraries
TARGET_LINK_LIBRARIES(ofxTest glut GLU ${dep_LIBRARY})

