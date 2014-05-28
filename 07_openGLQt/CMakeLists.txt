cmake_minimum_required(VERSION 2.6)

#QT4 usage
FIND_PACKAGE(Qt4)
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
INCLUDE(${QT_USE_FILE})

# MOC_HDRS set to all files with Q_OBJECT in it
SET(MOC_HDRS  glwidget.h  window.h)

# Regular cpp sources
SET(CPP_SRCS  glwidget.cpp  window.cpp)

# run moc on header files with Q_OBJECT
QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} )

#Builds up all
ADD_EXECUTABLE(myGui main.cpp ${CPP_SRCS} ${MOC_SRCS})

#Link libraries
TARGET_LINK_LIBRARIES(myGui ${QT_LIBRARIES} GLU)
