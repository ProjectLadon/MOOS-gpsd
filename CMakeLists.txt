#--------------------------------------------------------
# The CMakeLists.txt for:                         pGPSd
# Author(s):                                Pierce Nichols
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  SET(SYSTEM_LIBS
    wsock32)
else (${WIN32})
  SET(SYSTEM_LIBS
    m
    pthread)
endif (${WIN32})

SET(SRC
   PGPSD_MOOSApp.cpp  
   PGPSD_Info.cpp
   main.cpp
)  

ADD_EXECUTABLE(pGPSd ${SRC}	)

TARGET_LINK_LIBRARIES(pGPSd
   ${MOOS_LIBRARIES}
   mbutil
   ${SYSTEM_LIBS})

