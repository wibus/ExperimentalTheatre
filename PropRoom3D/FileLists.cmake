## Headers ##
# Art Director
SET(PROP3_ART_DIRECTOR_HEADERS
    ${PROP3_SRC_DIR}/Team/ArtDirector/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/QGlPostProdUnit.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracer.h)

# Choreographer
SET(PROP3_CHOREOGRAPHER_HEADERS
    ${PROP3_SRC_DIR}/Team/Choreographer/AbstractChoreographer.h
    ${PROP3_SRC_DIR}/Team/Choreographer/StdChoreographer.h)

# Designer
SET(PROP3_DESIGNER_HEADERS
    ${PROP3_SRC_DIR}/Team/Designer/AbstractDesigner.h
    ${PROP3_SRC_DIR}/Team/Designer/StdDesigner.h)

# Team
SET(PROP3_TEAM_HEADERS
    ${PROP3_ART_DIRECTOR_HEADERS}
    ${PROP3_CHOREOGRAPHER_HEADERS}
    ${PROP3_DESIGNER_HEADERS}
    ${PROP3_SRC_DIR}/Team/AbstractTeam.h
    ${PROP3_SRC_DIR}/Team/AbstractTeamMember.h
    ${PROP3_SRC_DIR}/Team/StdGlTeam.h)

# Costumes
SET(PROP3_COSTUME_HEADERS
    ${PROP3_SRC_DIR}/Prop/Costume/Costume.h
    ${PROP3_SRC_DIR}/Prop/Costume/Glass.h
    ${PROP3_SRC_DIR}/Prop/Costume/Chrome.h
    ${PROP3_SRC_DIR}/Prop/Costume/FlatPaint.h
    ${PROP3_SRC_DIR}/Prop/Costume/TexturedPaint.h)

# Hardwares
SET(PROP3_HARDWARE_HEADERS
    ${PROP3_SRC_DIR}/Prop/Hardware/Hardware.h)

# Volumes
SET(PROP3_VOLUME_HEADERS
    ${PROP3_SRC_DIR}/Prop/Volume/Raycast.h
    ${PROP3_SRC_DIR}/Prop/Volume/Volume.h
    ${PROP3_SRC_DIR}/Prop/Volume/Plane.h
    ${PROP3_SRC_DIR}/Prop/Volume/Sphere.h)

# Prop
SET(PROP3_PROP_HEADERS
    ${PROP3_COSTUME_HEADERS}
    ${PROP3_HARDWARE_HEADERS}
    ${PROP3_VOLUME_HEADERS}
    ${PROP3_SRC_DIR}/Prop/Prop.h)

# All the header files #
SET(PROP3_HEADERS
    ${PROP3_TEAM_HEADERS}
    ${PROP3_PROP_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Art Director
SET(PROP3_ART_DIRECTOR_SOURCES
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/QGlPostProdUnit.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracer.cpp)

# Choreographer
SET(PROP3_CHOREOGRAPHER_SOURCES
    ${PROP3_SRC_DIR}/Team/Choreographer/StdChoreographer.cpp)

# Designer
SET(PROP3_DESIGNER_SOURCES
    ${PROP3_SRC_DIR}/Team/Designer/StdDesigner.cpp)

# Team
SET(PROP3_TEAM_SOURCES
    ${PROP3_ART_DIRECTOR_SOURCES}
    ${PROP3_CHOREOGRAPHER_SOURCES}
    ${PROP3_DESIGNER_SOURCES}
    ${PROP3_SRC_DIR}/Team/AbstractTeam.cpp
    ${PROP3_SRC_DIR}/Team/StdGlTeam.cpp)

# Costumes
SET(PROP3_COSTUME_SOURCES
    ${PROP3_SRC_DIR}/Prop/Costume/Glass.cpp
    ${PROP3_SRC_DIR}/Prop/Costume/Chrome.cpp
    ${PROP3_SRC_DIR}/Prop/Costume/FlatPaint.cpp
    ${PROP3_SRC_DIR}/Prop/Costume/TexturedPaint.cpp)

# Hardwares
SET(PROP3_HARDWARE_SOURCES
    ${PROP3_SRC_DIR}/Prop/Hardware/Hardware.cpp)

# Volumes
SET(PROP3_VOLUME_SOURCES
    ${PROP3_SRC_DIR}/Prop/Volume/Volume.cpp
    ${PROP3_SRC_DIR}/Prop/Volume/Plane.cpp
    ${PROP3_SRC_DIR}/Prop/Volume/Sphere.cpp)

# Props
SET(PROP3_PROP_SOURCES
    ${PROP3_COSTUME_SOURCES}
    ${PROP3_HARDWARE_SOURCES}
    ${PROP3_VOLUME_SOURCES}
    ${PROP3_SRC_DIR}/Prop/Prop.cpp)

# All the source files #
SET(PROP3_SOURCES
    ${PROP3_TEAM_SOURCES}
    ${PROP3_PROP_SOURCES})
    

## Resources
SET(PROP3_RCC_FILES
    ${PROP3_SRC_DIR}/resources/PropRoom3D_Resources.qrc)
SET(PROP3_SHADER_FILES
    ${PROP3_SRC_DIR}/resources/shaders/clip_space.vert
    ${PROP3_SRC_DIR}/resources/shaders/post_prod.frag)
SET(PROP3_RESOURCE_FILES
    ${PROP3_RCC_FILES}
    ${PROP3_SHADER_FILES})

QT5_ADD_RESOURCES(PROP3_RCC_SRCS ${PROP3_RCC_FILES})


## Global ##
SET(PROP3_CONFIG_FILES
    ${PROP3_SRC_DIR}/CMakeLists.txt
    ${PROP3_SRC_DIR}/FileLists.cmake
    ${PROP3_SRC_DIR}/LibLists.cmake)

SET(PROP3_SRC_FILES
    ${PROP3_HEADERS}
    ${PROP3_SOURCES}
    ${PROP3_CONFIG_FILES}
    ${PROP3_RESOURCE_FILES}
    ${PROP3_RCC_SRCS})



## Source groups ##
SOURCE_GROUP("Header Files" FILES ${PROP3_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Costume" FILES ${PROP3_COSTUME_HEADERS})
SOURCE_GROUP("Header Files\\Hardware" FILES ${PROP3_HARDWARE_HEADERS})
SOURCE_GROUP("Header Files\\Prop Team" FILES ${PROP3_TEAM_HEADERS})
SOURCE_GROUP("Header Files\\Shape" FILES ${PROP3_SHAPE_HEADERS})

SOURCE_GROUP("Source Files" FILES ${PROP3_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Costume" FILES ${PROP3_COSTUME_SOURCES})
SOURCE_GROUP("Source Files\\Hardware" FILES ${PROP3_HARDWARE_SOURCES})
SOURCE_GROUP("Source Files\\Prop Team" FILES ${PROP3_TEAM_SOURCES})
SOURCE_GROUP("Source Files\\Shape" FILES ${PROP3_SHAPE_SOURCES})

SOURCE_GROUP("Config" FILES ${PROP3_CONFIG_FILES})

SOURCE_GROUP("Resources" FILES ${PROP3_RCC_FILES})
SOURCE_GROUP("Resources\\Shaders" FILES ${PROP3_SHADER_FILES})
