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


# Graphics
SET(CELLAR_GRAPHICS_CAMERA_HEADERS
    ${CELLAR_SRC_DIR}/Graphics/Camera/Camera.h
    ${CELLAR_SRC_DIR}/Graphics/Camera/CameraManBird.h
    ${CELLAR_SRC_DIR}/Graphics/Camera/CameraManFree.h)
    
SET(CELLAR_GRAPHICS_GL_HEADERS
    ${CELLAR_SRC_DIR}/Graphics/GL/GLFFmpegCamcorder.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlInputsOutputs.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlProgram.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlShaderBank.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlShader.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlToolkit.h
    ${CELLAR_SRC_DIR}/Graphics/GL/GlVao.h)

SET(CELLAR_GRAPHICS_IMAGE_HEADERS
    ${CELLAR_SRC_DIR}/Graphics/Image/Image.h
    ${CELLAR_SRC_DIR}/Graphics/Image/ImageBank.h)

SET(CELLAR_GRAPHICS_LIGHT_HEADERS
    ${CELLAR_SRC_DIR}/Graphics/Light/Light3D.h)

SET(CELLAR_GRAPHICS_HEADERS
    ${CELLAR_GRAPHICS_CAMERA_HEADERS}
    ${CELLAR_GRAPHICS_GL_HEADERS}
    ${CELLAR_GRAPHICS_IMAGE_HEADERS}
    ${CELLAR_GRAPHICS_LIGHT_HEADERS})

    
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
    ${CELLAR_GRAPHICS_HEADERS}
    ${CELLAR_MISC_HEADERS}
    ${CELLAR_SRC_DIR}/libCellarWorkbench_global.h)



## Sources ##
# Algorithm
SET(CELLAR_ALGORITHM_SOURCES
    ${CELLAR_SRC_DIR}/Algorithm/Noise.cpp)

    
# Data structure
SET(CELLAR_DATA_STRUCTURE_SOURCES)


# Date and time
SET(CELLAR_DATE_AND_TIME_SOURCES
    ${CELLAR_SRC_DIR}/DateAndTime/AbstractClock.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/Calendar.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/CClock.cpp
    ${CELLAR_SRC_DIR}/DateAndTime/QtClock.cpp)

    
# Design pattern
SET(CELLAR_DESIGN_PATTERN_SOURCES)


# Geometry
SET(CELLAR_GEOMETRY_SOURCES)


# Graphics
SET(CELLAR_GRAPHICS_CAMERA_SOURCES
    ${CELLAR_SRC_DIR}/Graphics/Camera/Camera.cpp
    ${CELLAR_SRC_DIR}/Graphics/Camera/CameraManBird.cpp
    ${CELLAR_SRC_DIR}/Graphics/Camera/CameraManFree.cpp)
    
SET(CELLAR_GRAPHICS_GL_SOURCES
    ${CELLAR_SRC_DIR}/Graphics/GL/GLFFmpegCamcorder.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlInputsOutputs.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlProgram.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlShaderBank.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlShader.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlToolkit.cpp
    ${CELLAR_SRC_DIR}/Graphics/GL/GlVao.cpp)

SET(CELLAR_GRAPHICS_IMAGE_SOURCES
    ${CELLAR_SRC_DIR}/Graphics/Image/Image.cpp
    ${CELLAR_SRC_DIR}/Graphics/Image/ImageBank.cpp)

SET(CELLAR_GRAPHICS_SOURCES
    ${CELLAR_GRAPHICS_CAMERA_SOURCES}
    ${CELLAR_GRAPHICS_GL_SOURCES}
    ${CELLAR_GRAPHICS_IMAGE_SOURCES})

    
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
    ${CELLAR_GRAPHICS_SOURCES}
    ${CELLAR_MISC_SOURCES})
    
    

## Global ##
SET(CELLAR_SRC_FILES
    ${CELLAR_HEADERS}
    ${CELLAR_SOURCES})

SET(CELLAR_CONFIG_FILES
    ${CELLAR_SRC_DIR}/CMakeLists.txt
    ${CELLAR_SRC_DIR}/FileLists.cmake
    ${CELLAR_SRC_DIR}/LibLists.cmake)

SET(CELLAR_INCLUDE_DIR
    ${CELLAR_SRC_DIR})
SET(CELLAR_INCLUDE_DIR
    ${CELLAR_INCLUDE_DIR}
    PARENT_SCOPE)


## Source groups ##
FILE(GLOB CELLAR_HEADERS_FILTER RELATIVE ${CELLAR_HEADERS})
SOURCE_GROUP("Header Files" FILES ${CELLAR_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Algorithm" FILES ${CELLAR_ALGORITHM_HEADERS})
SOURCE_GROUP("Header Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_HEADERS})
SOURCE_GROUP("Header Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_HEADERS})
SOURCE_GROUP("Header Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_HEADERS})
SOURCE_GROUP("Header Files\\Geometry" FILES ${CELLAR_GEOMETRY_HEADERS})
SOURCE_GROUP("Header Files\\Graphics" FILES ${CELLAR_GRAPHICS_HEADERS})
SOURCE_GROUP("Header Files\\Graphics\\Camera" FILES ${CELLAR_GRAPHICS_CAMERA_HEADERS})
SOURCE_GROUP("Header Files\\Graphics\\GL" FILES ${CELLAR_GRAPHICS_GL_HEADERS})
SOURCE_GROUP("Header Files\\Graphics\\Image" FILES ${CELLAR_GRAPHICS_IMAGE_HEADERS})
SOURCE_GROUP("Header Files\\Graphics\\Light" FILES ${CELLAR_GRAPHICS_Light_HEADERS})
SOURCE_GROUP("Header Files\\Misc" FILES ${CELLAR_MISC_HEADERS})

FILE(GLOB CELLAR_SOURCES_FILTER RELATIVE ${CELLAR_SOURCES})
SOURCE_GROUP("Source Files" FILES ${CELLAR_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Algorithm" FILES ${CELLAR_ALGORITHM_SOURCES})
SOURCE_GROUP("Source Files\\Data Structure" FILES ${CELLAR_DATA_STRUCTURE_SOURCES})
SOURCE_GROUP("Source Files\\Date And Time" FILES ${CELLAR_DATE_AND_TIME_SOURCES})
SOURCE_GROUP("Source Files\\Design Pattern" FILES ${CELLAR_DESIGN_PATTERN_SOURCES})
SOURCE_GROUP("Source Files\\Geometry" FILES ${CELLAR_GEOMETRY_SOURCES})
SOURCE_GROUP("Source Files\\Graphics" FILES ${CELLAR_GRAPHICS_SOURCES})
SOURCE_GROUP("Source Files\\Graphics\\Camera" FILES ${CELLAR_GRAPHICS_CAMERA_SOURCES})
SOURCE_GROUP("Source Files\\Graphics\\GL" FILES ${CELLAR_GRAPHICS_GL_SOURCES})
SOURCE_GROUP("Source Files\\Graphics\\Image" FILES ${CELLAR_GRAPHICS_IMAGE_SOURCES})
SOURCE_GROUP("Source Files\\Misc" FILES ${CELLAR_MISC_SOURCES})

SOURCE_GROUP("Config" FILES CELLAR_CONFIG_FILES)
