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

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/include 
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/include/ivp)

SET(MOOS_LIBRARIES
  MOOS
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/lib/libapputil.a
  ${CMAKE_CURRENT_SOURCE_DIR}/../moos-ivp/lib/libmbutil.a)

TARGET_LINK_LIBRARIES(pGPSd
  ${MOOS_LIBRARIES}
  m
  pthread
  Geographic
  gps)

