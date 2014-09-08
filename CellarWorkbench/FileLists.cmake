## Headers ##
# Algorithm
SET(CELLAR_ALGORITHM_HEADERS 
    ${CELLAR_SRC_DIR}/Algorithm/Noise.h)

    
# Data structure
SET(CELLAR_DATA_STRUCTURE_HEADERS
    ${CELLAR_SRC_DIR}/DataStructure/Barycenter.h
    ${CELLAR_SRC_DIR}/DataStructure/Grid2D.h
    ${CELLAR_SRC_DIR}/DataStructure/Grid3D.h
    ${CELLAR_SRC_DIR}/DataStructure/Matrix.h
    ${CELLAR_SRC_DIR}/DataStructure/MatrixStack.h
    ${CELLAR_SRC_DIR}/DataStructure/ObservableMatrixStack.h
    ${CELLAR_SRC_DIR}/DataStructure/PGrid2D.h
    ${CELLAR_SRC_DIR}/DataStructure/PGrid3D.h
    ${CELLAR_SRC_DIR}/DataStructure/Vector.h
    ${CELLAR_SRC_DIR}/DataStructure/VectorSpace.h)


# Date and time
SET(CELLAR_DATE_AND_TIME_HEADERS
    ${CELLAR_SRC_DIR}/DateAndTime/AbstractClock.h
    ${CELLAR_SRC_DIR}/DateAndTime/Calendar.h
    ${CELLAR_SRC_DIR}/DateAndTime/CClock.h
    ${CELLAR_SRC_DIR}/DateAndTime/QtClock.h
    ${CELLAR_SRC_DIR}/DateAndTime/Timer.h)

    
# Design pattern
SET(CELLAR_DESIGN_PATTERN_HEADERS
    ${CELLAR_SRC_DIR}/DesignPattern/Multiton.h
    ${CELLAR_SRC_DIR}/DesignPattern/Singleton.h
    ${CELLAR_SRC_DIR}/DesignPattern/SpecificObserver.h
    ${CELLAR_SRC_DIR}/DesignPattern/SpecificSubject.h)


# Geometry
SET(CELLAR_GEOMETRY_HEADERS
    ${CELLAR_SRC_DIR}/Geometry/Plane3D.h
    ${CELLAR_SRC_DIR}/Geometry/Segment2D.h
    ${CELLAR_SRC_DIR}/Geometry/Segment3D.h)

    
#Misc
SET(CELLAR_MISC_HEADERS
    ${CELLAR_SRC_DIR}/Misc/CellarUtils.h
    ${CELLAR_SRC_DIR}/Misc/Log.h)


# All the header files #
SET(CELLAR_HEADERS
    ${CELLAR_ALGORITHM_HEADERS}
    ${CELLAR_DATA_STRUCTURE_HEADERS}
    ${CELLAR_DATE_AND_TIME_HEADERS}
    ${CELLAR_DESIGN_PATTERN_HEADERS}
    ${CELLAR_GEOMETRY_HEADERS}
    ${CELLAR_GL3W_HEADERS}
    ${CELLAR_GRAPHICS_HEADERS}
    ${CELLAR_GRAPHICS_CAMERA_HEADERS}
    ${CELLAR_GRAPHICS_GL_HEADERS}
    ${CELLAR_GRAPHICS_IMAGE_HEADERS}
    ${CELLAR_GRAPHICS_LIGHT_HEADERS}
    ${CELLAR_MISC_HEADERS}
    ${CELLAR_SRC_DIR}/libCellarWorkbench_global.h)



## Sources ##
# Algorithm
SET(CELLAR_ALGORITHM_SOURCES
    ${CELLAR_SRC_DIR}/Algorithm/Noise.cpp)

    
# Data structure
SET(CELLAR_DATA_STRUCTURE_SOURCES
    ${CELLAR_SRC_DIR}/DataStructure/Barycenter.cpp)


# Date and time
SET(CELLAR_DATE_AND_TIME_SOURCES
    ${CELLAR_SRC_DIR}/DateAndTime/AbstractClock.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/Calendar.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/CClock.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/QtClock.cpp
	${CELLAR_SRC_DIR}/DateAndTime/Timer.cpp)

    
# Design pattern
SET(CELLAR_DESIGN_PATTERN_SOURCES)


# Geometry
SET(CELLAR_GEOMETRY_SOURCES)

    
#Misc
SET(CELLAR_MISC_SOURCES
    ${CELLAR_SRC_DIR}/Misc/CellarUtils.cpp
    ${CELLAR_SRC_DIR}/Misc/Log.cpp)


# All the source files #
SET(CELLAR_SOURCES
    ${CELLAR_ALGORITHM_SOURCES}
    ${CELLAR_DATA_STRUCTURE_SOURCES}
    ${CELLAR_DATE_AND_TIME_SOURCES}
    ${CELLAR_DESIGN_PATTERN_SOURCES}
    ${CELLAR_GEOMETRY_SOURCES}
    ${CELLAR_GL3W_SOURCES}
    ${CELLAR_GRAPHICS_SOURCES}
    ${CELLAR_GRAPHICS_CAMERA_SOURCES}
    ${CELLAR_GRAPHICS_GL_SOURCES}
    ${CELLAR_GRAPHICS_IMAGE_SOURCES}
    ${CELLAR_GRAPHICS_LIGHT_SOURCES}
    ${CELLAR_MISC_SOURCES})
    
    

## Global ##
SET(CELLAR_CONFIG_FILES
    ${CELLAR_SRC_DIR}/CMakeLists.txt
    ${CELLAR_SRC_DIR}/FileLists.cmake
    ${CELLAR_SRC_DIR}/LibLists.cmake)
	
SET(CELLAR_SRC_FILES
    ${CELLAR_HEADERS}
    ${CELLAR_SOURCES}
    ${CELLAR_CONFIG_FILES})



## Source groups ##
SOURCE_GROUP("Header Files" FILES ${CELLAR_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Algorithm" FILES ${CELLAR_ALGORITHM_HEADERS})
SOURCE_GROUP("Header Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_HEADERS})
SOURCE_GROUP("Header Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_HEADERS})
SOURCE_GROUP("Header Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_HEADERS})
SOURCE_GROUP("Header Files\\Geometry" FILES ${CELLAR_GEOMETRY_HEADERS})
SOURCE_GROUP("Header Files\\Misc" FILES ${CELLAR_MISC_HEADERS})

SOURCE_GROUP("Source Files" FILES ${CELLAR_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Algorithm" FILES ${CELLAR_ALGORITHM_SOURCES})
SOURCE_GROUP("Source Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_SOURCES})
SOURCE_GROUP("Source Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_SOURCES})
SOURCE_GROUP("Source Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_SOURCES})
SOURCE_GROUP("Source Files\\Geometry" FILES ${CELLAR_GEOMETRY_SOURCES})
SOURCE_GROUP("Source Files\\Misc" FILES ${CELLAR_MISC_SOURCES})

SOURCE_GROUP("Config" FILES ${CELLAR_CONFIG_FILES})
