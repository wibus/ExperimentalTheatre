## Headers ##
# Costumes
SET(PROP3_COSTUME_HEADERS
    ${PROP3_SRC_DIR}/Costume/AbstractCostume.h)

# Materials
SET(PROP3_MATERIAL_HEADERS
    ${PROP3_SRC_DIR}/Material/Material.h)
  
# Prop team
SET(PROP3_PROP_TEAM_HEADERS
    ${PROP3_SRC_DIR}/PropTeam/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractChoreographer.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropDesigner.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropTeam.h
    ${PROP3_SRC_DIR}/PropTeam/AbstractTeamMember.h
    ${PROP3_SRC_DIR}/PropTeam/StdPropDesigner.h)
    
# Shape
SET(PROP3_SHAPE_HEADERS
    ${PROP3_SRC_DIR}/Shape/AbstractShape.h)

# All the header files #
SET(PROP3_HEADERS
    ${PROP3_COSTUME_HEADERS}
    ${PROP3_MATERIAL_HEADERS}
    ${PROP3_PROP_TEAM_HEADERS}
    ${PROP3_SHAPE_HEADERS}
    ${PROP3_SRC_DIR}/AbstractProp.h  
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Costumes
SET(PROP3_COSTUME_SOURCES
    ${PROP3_SRC_DIR}/Costume/AbstractCostume.cpp)

# Materials
SET(PROP3_MATERIAL_SOURCES
    ${PROP3_SRC_DIR}/Material/Material.cpp)
    
# Prop Team
SET(PROP3_PROP_TEAM_SOURCES
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropTeam.cpp
    ${PROP3_SRC_DIR}/PropTeam/StdPropDesigner.cpp)
    
# Shape
SET(PROP3_SHAPE_SOURCES
    ${PROP3_SRC_DIR}/Shape/AbstractShape.cpp)

# All the source files #
SET(PROP3_SOURCES
    ${PROP3_COSTUME_SOURCES}
    ${PROP3_MATERIAL_SOURCES}
    ${PROP3_PROP_TEAM_SOURCES}
    ${PROP3_SHAPE_SOURCES}
    ${PROP3_SRC_DIR}/AbstractProp.cpp)
    

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
SOURCE_GROUP("Header Files\\Material" FILES ${PROP3_MATERIAL_HEADERS})
SOURCE_GROUP("Header Files\\Prop Team" FILES ${PROP3_PROP_TEAM_HEADERS})
SOURCE_GROUP("Header Files\\Shape" FILES ${PROP3_SHAPE_HEADERS})

SOURCE_GROUP("Source Files" FILES ${PROP3_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Costume" FILES ${PROP3_COSTUME_SOURCES})
SOURCE_GROUP("Source Files\\Material" FILES ${PROP3_MATERIAL_SOURCES})
SOURCE_GROUP("Source Files\\Prop Team" FILES ${PROP3_PROP_TEAM_SOURCES})
SOURCE_GROUP("Source Files\\Shape" FILES ${PROP3_SHAPE_SOURCES})

SOURCE_GROUP("Config" FILES ${PROP3_CONFIG_FILES})

SOURCE_GROUP("Resources" FILES ${PROP3_RCC_FILES})
SOURCE_GROUP("Resources\\Shaders" FILES ${PROP3_SHADER_FILES})
