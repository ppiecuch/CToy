## Find source and headers
FILE(GLOB HEADERS
  *.h
  imgui/*.h
)
FILE(GLOB SOURCES
  *.c*
  imgui/*.c*
)

IF(CMAKE_HAS_SORT)
  LIST(SORT HEADERS)
  LIST(SORT SOURCES)
ENDIF(CMAKE_HAS_SORT)

## Build
ADD_DEFINITIONS(-DCIMGUI_EXPORT)
ADD_LIBRARY(cimgui SHARED ${SOURCES} ${HEADERS})

## Install for Editor
INSTALL(TARGETS cimgui DESTINATION ${MTCL_INSTALL_DIR}/lib)

IF(APPLE)
  SET_TARGET_PROPERTIES(
    cimgui
    PROPERTIES
    INSTALL_NAME_DIR "@rpath"
  )
ENDIF(APPLE)
