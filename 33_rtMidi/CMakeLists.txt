
cmake_minimum_required(VERSION 2.6)

#preprocessor definition
ADD_DEFINITIONS(-D__LINUX_ALSA__)
ADD_DEFINITIONS(-D__RTMIDI_DEBUG__)

#find alsa library
SET ( ALSA_LIB_NAME "asound" )
FIND_LIBRARY ( ALSA_LIBRARY
  NAMES "${ALSA_LIB_NAME}"
  PATHS "/usr/local/lib" "/usr/lib"
)

#set header directories
# INCLUDE_DIRECTORIES(${ALSA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(/home/andreu/dev/extSw/rtmidi-2.0.1)

#build executable
ADD_EXECUTABLE(midiout midiout.cpp /home/andreu/dev/extSw/rtmidi-2.0.1/RtMidi.cpp)
ADD_EXECUTABLE(midiprobe midiprobe.cpp /home/andreu/dev/extSw/rtmidi-2.0.1/RtMidi.cpp)

#link
TARGET_LINK_LIBRARIES(midiout ${ALSA_LIB_NAME})
TARGET_LINK_LIBRARIES(midiprobe ${ALSA_LIB_NAME})
