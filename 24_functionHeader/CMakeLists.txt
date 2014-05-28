
#Etiquetem tots els *.cpp amb el nom de MY_SOURCES
SET(MY_SOURCES myFunctions.cpp)

#Creem un executable a partir del main de myMain1.cpp i el codi de MY_SOURCES
ADD_EXECUTABLE(myMain1 myMain1.cpp ${MY_SOURCES})

#Creem un executable a partir del main de myMain2.cpp i el codi de MY_SOURCES
ADD_EXECUTABLE(myMain2 myMain2.cpp ${MY_SOURCES})

