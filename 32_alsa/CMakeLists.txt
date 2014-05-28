cmake_minimum_required(VERSION 2.6)

#find required packages

IF ( ALSA_INCLUDE_DIR )
  SET ( ALSA_FIND_QUIETLY TRUE )
ENDIF ( ALSA_INCLUDE_DIR )

SET ( ALSA_HDR_NAME 
	"alsa/asoundlib.h" )

FIND_PATH ( ALSA_INCLUDE_DIR 
	"${ALSA_HDR_NAME}"
	"/usr/local/include" "/usr/include")

SET ( ALSA_LIB_NAME 	
	"asound" )

FIND_LIBRARY ( ALSA_LIBRARY
  NAMES "${ALSA_LIB_NAME}"
  PATHS "/usr/local/lib" "/usr/lib"
)

# FIND_PACKAGE(asound REQUIRED)

#set header directories
INCLUDE_DIRECTORIES(${ALSA_INCLUDE_DIR})

#Builds up all
ADD_EXECUTABLE(alsaMain alsaMain.cpp)
ADD_EXECUTABLE(alsaCountWvFm alsaCountWvFm.cpp)
ADD_EXECUTABLE(alsaList alsaList.cpp)


#Link libraries
TARGET_LINK_LIBRARIES(alsaMain ${ALSA_LIB_NAME})
TARGET_LINK_LIBRARIES(alsaCountWvFm ${ALSA_LIB_NAME})
TARGET_LINK_LIBRARIES(alsaList ${ALSA_LIB_NAME})

