#--------------------------------------------------------
# The CMakeLists.txt for:                       pGPSd
# Author(s):
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(SRC
  GPSd.cpp
  GPSd_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pGPSd ${SRC})

TARGET_LINK_LIBRARIES(pGPSd
  ${MOOS_LIBRARIES}
  mbutil
  apputil
  m
  pthread
  Geographic
gps)

find_program(CTAGS ctags)
if (CTAGS)
	FIND_FILE(MAKE_CTAGS make_ctags.sh ../..)
    ADD_CUSTOM_TARGET(gpsd_ctags ALL
        ${MAKE_CTAGS} .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS pGPSd
    )
endif(CTAGS)
