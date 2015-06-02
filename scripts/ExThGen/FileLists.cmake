## Headers ##

# All the header files #
SET(PROJECTNAME_HEADERS
    ${PROJECTNAME_SRC_DIR}/PROJECTNAMECharacter.h)



## Sources ##

# All the source files #
SET(PROJECTNAME_SOURCES
    ${PROJECTNAME_SRC_DIR}/PROJECTNAMECharacter.cpp
    ${PROJECTNAME_SRC_DIR}/main.cpp)



## Global ##
SET(PROJECTNAME_CONFIG_FILES
    ${PROJECTNAME_SRC_DIR}/CMakeLists.txt
    ${PROJECTNAME_SRC_DIR}/FileLists.cmake
    ${PROJECTNAME_SRC_DIR}/LibLists.cmake)
	
SET(PROJECTNAME_SRC_FILES
    ${PROJECTNAME_HEADERS}
    ${PROJECTNAME_SOURCES}
    ${PROJECTNAME_CONFIG_FILES}
    ${PROJECTNAME_MOC_CPP_FILES})
