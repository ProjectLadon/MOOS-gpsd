#--------------------------------------------------------
# The CMakeLists.txt for:                       pGPSd
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  GPSd.cpp
  GPSd_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pGPSd ${SRC})

TARGET_LINK_LIBRARIES(pGPSd
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread
   Geographic
   gps)

