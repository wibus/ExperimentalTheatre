SET(PROP3_PROJECT PropRoom3D)
SET(PROP3_PROJECT ${PROP3_PROJECT} PARENT_SCOPE)
MESSAGE(STATUS "Building ${PROP3_PROJECT} library")
PROJECT(${PROP3_PROJECT} CXX)

SET(PROP3_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(PROP3_SRC_DIR ${PROP3_SRC_DIR} PARENT_SCOPE)
MESSAGE(STATUS "${PROP3_PROJECT} src dir: ${PROP3_SRC_DIR}")
SET(PROP3_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "${PROP3_PROJECT} bin dir: ${PROP3_BIN_DIR}")
SET(PROP3_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "${PROP3_PROJECT} install dir: ${PROP3_INSTALL_DIR}")


INCLUDE(LibLists.cmake)
INCLUDE(FileLists.cmake)


MESSAGE(STATUS "${PROP3_PROJECT} libraires: ${PROP3_LIBRARIES}")
MESSAGE(STATUS "${PROP3_PROJECT} Qt modules: ${PROP3_QT_MODULES}")
MESSAGE(STATUS "${PROP3_PROJECT} include dirs: ${PROP3_INCLUDE_DIR}")


ADD_DEFINITIONS(-DPROPROOM3D_LIBRARY)

ADD_LIBRARY(${PROP3_PROJECT} ${LIB_TYPE} ${PROP3_SRC_FILES})
TARGET_LINK_LIBRARIES(${PROP3_PROJECT} ${PROP3_LIBRARIES})
QT5_USE_MODULES(${PROP3_PROJECT} ${PROP3_QT_MODULES})
INCLUDE_DIRECTORIES(${PROP3_INCLUDE_DIR})

INSTALL(TARGETS ${PROP3_PROJECT}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY ${PROP3_SRC_DIR}/
        DESTINATION include/${EXTH_INSTALL_DIR}/${PROP3_PROJECT}
        FILES_MATCHING PATTERN "*.h")
