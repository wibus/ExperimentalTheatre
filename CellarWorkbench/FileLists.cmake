## Headers ##

# Camera
SET(CELLAR_CAMERA_HEADERS
    ${CELLAR_SRC_DIR}/Camera/Camera.h
    ${CELLAR_SRC_DIR}/Camera/CameraManBird.h
    ${CELLAR_SRC_DIR}/Camera/CameraManFree.h)


# Data structure
SET(CELLAR_DATA_STRUCTURE_HEADERS
    ${CELLAR_SRC_DIR}/DataStructure/Grid2D.h
    ${CELLAR_SRC_DIR}/DataStructure/Grid3D.h
    ${CELLAR_SRC_DIR}/DataStructure/PGrid2D.h
    ${CELLAR_SRC_DIR}/DataStructure/PGrid3D.h)


# Date and time
SET(CELLAR_DATE_AND_TIME_HEADERS
    ${CELLAR_SRC_DIR}/DateAndTime/Calendar.h
    ${CELLAR_SRC_DIR}/DateAndTime/Clock.h)

    
# Design pattern
SET(CELLAR_DESIGN_PATTERN_HEADERS
    ${CELLAR_SRC_DIR}/DesignPattern/Singleton.h
    ${CELLAR_SRC_DIR}/DesignPattern/SpecificObserver.h
    ${CELLAR_SRC_DIR}/DesignPattern/SpecificSubject.h)


# GL
SET(CELLAR_GL_HEADERS
    ${CELLAR_SRC_DIR}/GL/GLFFmpegCamcorder.h
    ${CELLAR_SRC_DIR}/GL/GlInputsOutputs.h
    ${CELLAR_SRC_DIR}/GL/GlProgram.h
    ${CELLAR_SRC_DIR}/GL/GlShaderBank.h
    ${CELLAR_SRC_DIR}/GL/GlShader.h
    ${CELLAR_SRC_DIR}/GL/GlToolkit.h
    ${CELLAR_SRC_DIR}/GL/GlVao.h)


# Image
SET(CELLAR_IMAGE_HEADERS
    ${CELLAR_SRC_DIR}/Image/Image.h
    ${CELLAR_SRC_DIR}/Image/ImageBank.h
    ${CELLAR_SRC_DIR}/Image/ImageSampler.h)

    
# Misc
SET(CELLAR_MISC_HEADERS
    ${CELLAR_SRC_DIR}/Misc/Log.h
    ${CELLAR_SRC_DIR}/Misc/Distribution.h
    ${CELLAR_SRC_DIR}/Misc/SimplexNoise.h
    ${CELLAR_SRC_DIR}/Misc/StringUtils.h)


# Path
SET(CELLAR_PATH_HEADERS
    ${CELLAR_SRC_DIR}/Path/AbstractPath.h
    ${CELLAR_SRC_DIR}/Path/LinearPath.h
    ${CELLAR_SRC_DIR}/Path/CubicSplinePath.h
    ${CELLAR_SRC_DIR}/Path/CompositePath.h)


# All the header files #
SET(CELLAR_HEADERS
    ${CELLAR_CAMERA_HEADERS}
    ${CELLAR_DATA_STRUCTURE_HEADERS}
    ${CELLAR_DATE_AND_TIME_HEADERS}
    ${CELLAR_DESIGN_PATTERN_HEADERS}
    ${CELLAR_GL_HEADERS}
    ${CELLAR_IMAGE_HEADERS}
    ${CELLAR_MISC_HEADERS}
    ${CELLAR_PATH_HEADERS}
    ${CELLAR_SRC_DIR}/libCellarWorkbench_global.h)



## Sources ##

# Camera
SET(CELLAR_CAMERA_SOURCES
    ${CELLAR_SRC_DIR}/Camera/Camera.cpp
    ${CELLAR_SRC_DIR}/Camera/CameraManBird.cpp
    ${CELLAR_SRC_DIR}/Camera/CameraManFree.cpp)


# Data structure
SET(CELLAR_DATA_STRUCTURE_SOURCES)


# Date and time
SET(CELLAR_DATE_AND_TIME_SOURCES
    ${CELLAR_SRC_DIR}/DateAndTime/Calendar.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/Clock.cpp)

    
# Design pattern
SET(CELLAR_DESIGN_PATTERN_SOURCES)


# GL
SET(CELLAR_GL_SOURCES
    ${CELLAR_SRC_DIR}/GL/GLFFmpegCamcorder.cpp
    ${CELLAR_SRC_DIR}/GL/GlInputsOutputs.cpp
    ${CELLAR_SRC_DIR}/GL/GlProgram.cpp
    ${CELLAR_SRC_DIR}/GL/GlShaderBank.cpp
    ${CELLAR_SRC_DIR}/GL/GlShader.cpp
    ${CELLAR_SRC_DIR}/GL/GlToolkit.cpp
    ${CELLAR_SRC_DIR}/GL/GlVao.cpp)


# Image
SET(CELLAR_IMAGE_SOURCES
    ${CELLAR_SRC_DIR}/Image/Image.cpp
    ${CELLAR_SRC_DIR}/Image/ImageBank.cpp
    ${CELLAR_SRC_DIR}/Image/ImageSampler.cpp)

    
# Misc
SET(CELLAR_MISC_SOURCES
    ${CELLAR_SRC_DIR}/Misc/Log.cpp
    ${CELLAR_SRC_DIR}/Misc/Distribution.cpp
    ${CELLAR_SRC_DIR}/Misc/SimplexNoise.cpp
    ${CELLAR_SRC_DIR}/Misc/StringUtils.cpp)


# Path
SET(CELLAR_PATH_SOURCES)


# All the source files #
SET(CELLAR_SOURCES
    ${CELLAR_CAMERA_SOURCES}
    ${CELLAR_DATA_STRUCTURE_SOURCES}
    ${CELLAR_DATE_AND_TIME_SOURCES}
    ${CELLAR_DESIGN_PATTERN_SOURCES}
    ${CELLAR_GL_SOURCES}
    ${CELLAR_IMAGE_SOURCES}
    ${CELLAR_MISC_SOURCES}
    ${CELLAR_PATH_SOURCES})
    
    

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
SOURCE_GROUP("Header Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_HEADERS})
SOURCE_GROUP("Header Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_HEADERS})
SOURCE_GROUP("Header Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_HEADERS})
SOURCE_GROUP("Header Files\\Misc" FILES ${CELLAR_MISC_HEADERS})
SOURCE_GROUP("Header Files\\Camera" FILES ${CELLAR_CAMERA_HEADERS})
SOURCE_GROUP("Header Files\\GL" FILES ${CELLAR_GL_HEADERS})
SOURCE_GROUP("Header Files\\Image" FILES ${CELLAR_IMAGE_HEADERS})

SOURCE_GROUP("Source Files" FILES ${CELLAR_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_SOURCES})
SOURCE_GROUP("Source Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_SOURCES})
SOURCE_GROUP("Source Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_SOURCES})
SOURCE_GROUP("Source Files\\Misc" FILES ${CELLAR_MISC_SOURCES})
SOURCE_GROUP("Source Files\\Camera" FILES ${CELLAR_CAMERA_SOURCES})
SOURCE_GROUP("Source Files\\GL" FILES ${CELLAR_GL_SOURCES})
SOURCE_GROUP("Source Files\\Image" FILES ${CELLAR_IMAGE_SOURCES})

SOURCE_GROUP("Config" FILES ${CELLAR_CONFIG_FILES})
