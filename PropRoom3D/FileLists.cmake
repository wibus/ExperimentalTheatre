## Headers ##
# Prop team
SET(PROP3_PROP_TEAM_HEADERS
    ${PROP3_SRC_DIR}/PropTeam/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractChoreographer.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropDesigner.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropTeam.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractTeamMember.h
    ${PROP3_SRC_DIR}/PropTeam/CpuRaytracer.h
    ${PROP3_SRC_DIR}/PropTeam/StdGlPropTeam.h
    ${PROP3_SRC_DIR}/PropTeam/StdChoreographer.h
    ${PROP3_SRC_DIR}/PropTeam/StdPropDesigner.h)

# Costumes
SET(PROP3_COSTUME_HEADERS
    ${PROP3_SRC_DIR}/Prop/Costume/AbstractCostume.h
    ${PROP3_SRC_DIR}/Prop/Costume/FlatPaint.h)

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
    ${PROP3_PROP_TEAM_HEADERS}
    ${PROP3_PROP_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Prop Team
SET(PROP3_PROP_TEAM_SOURCES
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropTeam.cpp
    ${PROP3_SRC_DIR}/PropTeam/CpuRaytracer.cpp
    ${PROP3_SRC_DIR}/PropTeam/StdGlPropTeam.cpp
    ${PROP3_SRC_DIR}/PropTeam/StdChoreographer.cpp
    ${PROP3_SRC_DIR}/PropTeam/StdPropDesigner.cpp)

# Costumes
SET(PROP3_COSTUME_SOURCES
    ${PROP3_SRC_DIR}/Prop/Costume/FlatPaint.cpp)

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
    ${PROP3_PROP_TEAM_SOURCES}
    ${PROP3_PROP_SOURCES})
    

## Resources
SET(PROP3_RCC_FILES
    ${PROP3_SRC_DIR}/resources/PropRoom3D_Resources.qrc)
SET(PROP3_SHADER_FILES
    )
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
SOURCE_GROUP("Header Files\\Prop Team" FILES ${PROP3_PROP_TEAM_HEADERS})
SOURCE_GROUP("Header Files\\Shape" FILES ${PROP3_SHAPE_HEADERS})

SOURCE_GROUP("Source Files" FILES ${PROP3_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Costume" FILES ${PROP3_COSTUME_SOURCES})
SOURCE_GROUP("Source Files\\Hardware" FILES ${PROP3_HARDWARE_SOURCES})
SOURCE_GROUP("Source Files\\Prop Team" FILES ${PROP3_PROP_TEAM_SOURCES})
SOURCE_GROUP("Source Files\\Shape" FILES ${PROP3_SHAPE_SOURCES})

SOURCE_GROUP("Config" FILES ${PROP3_CONFIG_FILES})

SOURCE_GROUP("Resources" FILES ${PROP3_RCC_FILES})
SOURCE_GROUP("Resources\\Shaders" FILES ${PROP3_SHADER_FILES})
