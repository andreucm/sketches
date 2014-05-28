cmake_minimum_required(VERSION 2.6)

#find required packages
FIND_PACKAGE(OpenCV REQUIRED)

#set header directories
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})

#Builds up all
# ADD_EXECUTABLE(hwOpenCv helloWorldOpenCv.cpp)
ADD_EXECUTABLE(matMainOpenCV2 matMainOpenCV2.cpp)
ADD_EXECUTABLE(histogramsMain histogramsMain.cpp)
ADD_EXECUTABLE(histogramCalc histogramCalc.cpp)

#Link libraries
# TARGET_LINK_LIBRARIES(hwOpenCv ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(matMainOpenCV2 ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(histogramsMain ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(histogramCalc ${OpenCV_LIBS})