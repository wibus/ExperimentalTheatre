## Headers ##
SET(MEDIA_CAMERA_HEADERS
    ${MEDIA_SRC_DIR}/Camera/Camera.h
    ${MEDIA_SRC_DIR}/Camera/CameraManBird.h
    ${MEDIA_SRC_DIR}/Camera/CameraManFree.h)
	
SET(MEDIA_GL_HEADERS
    ${MEDIA_SRC_DIR}/GL/GLFFmpegCamcorder.h
    ${MEDIA_SRC_DIR}/GL/GlInputsOutputs.h
    ${MEDIA_SRC_DIR}/GL/GlProgram.h
    ${MEDIA_SRC_DIR}/GL/GlShaderBank.h
    ${MEDIA_SRC_DIR}/GL/GlShader.h
    ${MEDIA_SRC_DIR}/GL/GlToolkit.h
    ${MEDIA_SRC_DIR}/GL/GlVao.h)
	
SET(MEDIA_GL3W_HEADERS
    ${MEDIA_SRC_DIR}/GL3/gl3.h
    ${MEDIA_SRC_DIR}/GL3/gl3w.h)

SET(MEDIA_IMAGE_HEADERS
    ${MEDIA_SRC_DIR}/Image/Image.h
    ${MEDIA_SRC_DIR}/Image/ImageBank.h)

SET(MEDIA_LIGHT_HEADERS
    ${MEDIA_SRC_DIR}/Light/Light3D.h)

SET(MEDIA_TEXT_HEADERS
    ${MEDIA_SRC_DIR}/Text/DistanceFieldFont.h)
	
# All the header files #
SET(MEDIA_HEADERS
	${MEDIA_CAMERA_HEADERS}
	${MEDIA_GL_HEADERS}
	${MEDIA_GL3W_HEADERS}
	${MEDIA_IMAGE_HEADERS}
	${MEDIA_LIGHT_HEADERS}
    ${MEDIA_TEXT_HEADERS}
    ${MEDIA_SRC_DIR}/libMediaWorkbench_global.h)	


## Sources ##
SET(MEDIA_CAMERA_SOURCES
    ${MEDIA_SRC_DIR}/Camera/Camera.cpp
    ${MEDIA_SRC_DIR}/Camera/CameraManBird.cpp
    ${MEDIA_SRC_DIR}/Camera/CameraManFree.cpp)
    
SET(MEDIA_GL_SOURCES
    ${MEDIA_SRC_DIR}/GL/GLFFmpegCamcorder.cpp
    ${MEDIA_SRC_DIR}/GL/GlInputsOutputs.cpp
    ${MEDIA_SRC_DIR}/GL/GlProgram.cpp
    ${MEDIA_SRC_DIR}/GL/GlShaderBank.cpp
    ${MEDIA_SRC_DIR}/GL/GlShader.cpp
    ${MEDIA_SRC_DIR}/GL/GlToolkit.cpp
    ${MEDIA_SRC_DIR}/GL/GlVao.cpp)
	
SET(MEDIA_GL3W_SOURCES
    ${MEDIA_SRC_DIR}/GL3/gl3w.c)

SET(MEDIA_IMAGE_SOURCES
    ${MEDIA_SRC_DIR}/Image/Image.cpp
    ${MEDIA_SRC_DIR}/Image/ImageBank.cpp)

SET(MEDIA_LIGHT_SOURCES
    ${MEDIA_SRC_DIR}/Light/Light3D.cpp)

SET(MEDIA_TEXT_SOURCES
    ${MEDIA_SRC_DIR}/Text/DistanceFieldFont.cpp)

# All the source files #
SET(MEDIA_SOURCES
	${MEDIA_CAMERA_SOURCES}
	${MEDIA_GL_SOURCES}
	${MEDIA_GL3W_SOURCES}
	${MEDIA_IMAGE_SOURCES}
    ${MEDIA_LIGHT_SOURCES}
    ${MEDIA_TEXT_SOURCES})

	
## Global ##
SET(MEDIA_CONFIG_FILES
    ${MEDIA_SRC_DIR}/CMakeLists.txt
    ${MEDIA_SRC_DIR}/FileLists.cmake
    ${MEDIA_SRC_DIR}/LibLists.cmake)
	
SET(MEDIA_SRC_FILES
    ${MEDIA_HEADERS}
    ${MEDIA_SOURCES}
	${MEDIA_CONFIG_FILES})

	
SET(MEDIA_INCLUDE_DIR
    ${MEDIA_SRC_DIR})
SET(MEDIA_INCLUDE_DIR
    ${MEDIA_INCLUDE_DIR}
    PARENT_SCOPE)


## Source groups ##
SOURCE_GROUP("Header Files\\Camera" FILES ${MEDIA_CAMERA_HEADERS})
SOURCE_GROUP("Header Files\\GL" FILES ${MEDIA_GL_HEADERS})
SOURCE_GROUP("Header Files\\GL3W" FILES ${MEDIA_GL3W_HEADERS})
SOURCE_GROUP("Header Files\\Image" FILES ${MEDIA_IMAGE_HEADERS})
SOURCE_GROUP("Header Files\\Light" FILES ${MEDIA_LIGHT_HEADERS})
SOURCE_GROUP("Header Files\\Text" FILES ${MEDIA_TEXT_HEADERS})

SOURCE_GROUP("Source Files\\Camera" FILES ${MEDIA_CAMERA_SOURCES})
SOURCE_GROUP("Source Files\\GL" FILES ${MEDIA_GL_SOURCES})
SOURCE_GROUP("Source Files\\GL3" FILES ${MEDIA_GL3W_SOURCES})
SOURCE_GROUP("Source Files\\Image" FILES ${MEDIA_IMAGE_SOURCES})
SOURCE_GROUP("Source Files\\Light" FILES ${MEDIA_LIGHT_SOURCES})
SOURCE_GROUP("Source Files\\Text" FILES ${MEDIA_TEXT_SOURCES})

SOURCE_GROUP("Config" FILES ${MEDIA_CONFIG_FILES})
