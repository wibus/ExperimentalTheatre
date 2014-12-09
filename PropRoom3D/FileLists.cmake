## Headers ##
# Costumes
SET(PROP3_COSTUME_HEADERS
    ${PROP3_SRC_DIR}/Costume/AbstractCostume.h
    ${PROP3_SRC_DIR}/Costume/FlatPaint.h)

# Hardwares
SET(PROP3_HARDWARE_HEADERS
    ${PROP3_SRC_DIR}/Hardware/Hardware.h)
  
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
    ${PROP3_SRC_DIR}/Shape/AbstractShape.h
    ${PROP3_SRC_DIR}/Shape/Brep.h)

# Space equations
SET(PROP3_SHAPE_SPACE_EQUATION_HEADERS
    ${PROP3_SRC_DIR}/Shape/SpaceEquation/SpaceEquation.h
    ${PROP3_SRC_DIR}/Shape/SpaceEquation/QuadricEquation.h)

# All the header files #
SET(PROP3_HEADERS
    ${PROP3_COSTUME_HEADERS}
    ${PROP3_HARDWARE_HEADERS}
    ${PROP3_PROP_TEAM_HEADERS}
    ${PROP3_SHAPE_HEADERS}
    ${PROP3_SHAPE_SPACE_EQUATION_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Costumes
SET(PROP3_COSTUME_SOURCES
    ${PROP3_SRC_DIR}/Costume/FlatPaint.cpp)

# Hardwares
SET(PROP3_HARDWARE_SOURCES
    ${PROP3_SRC_DIR}/Hardware/Hardware.cpp)
    
# Prop Team
SET(PROP3_PROP_TEAM_SOURCES
    ${PROP3_SRC_DIR}/PropTeam/AbstractPropTeam.cpp
    ${PROP3_SRC_DIR}/PropTeam/StdPropDesigner.cpp)
    
# Shape
SET(PROP3_SHAPE_SOURCES
    ${PROP3_SRC_DIR}/Shape/AbstractShape.cpp
    ${PROP3_SRC_DIR}/Shape/Brep.cpp)

# Space equations
SET(PROP3_SHAPE_SPACE_EQUATION_SOURCES
    ${PROP3_SRC_DIR}/Shape/SpaceEquation/SpaceEquation.cpp
    ${PROP3_SRC_DIR}/Shape/SpaceEquation/QuadricEquation.cpp)

# All the source files #
SET(PROP3_SOURCES
    ${PROP3_COSTUME_SOURCES}
    ${PROP3_HARDWARE_SOURCES}
    ${PROP3_PROP_TEAM_SOURCES}
    ${PROP3_SHAPE_SOURCES}
    ${PROP3_SHAPE_SPACE_EQUATION_SOURCES})
    

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
