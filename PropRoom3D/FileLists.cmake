## Headers ##
# All the header files #
SET(PROP3_HEADERS
    ${PROP3_SRC_DIR}/AbstractProp.h
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)


## Sources ##
# All the source files #
SET(PROP3_SOURCES
    ${PROP3_SRC_DIR}/AbstractProp.cpp)


## Global ##
SET(PROP3_SRC_FILES
    ${PROP3_HEADERS}
    ${PROP3_SOURCES})

SET(PROP2_CONFIG_FILES
    ${PROP3_SRC_DIR}/CMakeLists.txt
    ${PROP3_SRC_DIR}/FileLists.txt
    ${PROP3_SRC_DIR}/LibLists.txt)

SET(PROP3_INCLUDE_DIR
    ${PROP3_SRC_DIR})
SET(PROP3_INCLUDE_DIR
    ${PROP3_INCLUDE_DIR}
    PARENT_SCOPE)
