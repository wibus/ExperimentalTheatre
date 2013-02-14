## Headers ##
# Costumes
SET(PROP2_COSTUME_HEADERS
    ${PROP2_SRC_DIR}/Costume/AbstractCostume.h
    ${PROP2_SRC_DIR}/Costume/CircleCostume.h
    ${PROP2_SRC_DIR}/Costume/PolygonCostume.h)
   
# Huds
SET(PROP2_HUD_HEADERS
    ${PROP2_SRC_DIR}/Hud/AbstractHud.h
    ${PROP2_SRC_DIR}/Hud/ImageHud.h
    ${PROP2_SRC_DIR}/Hud/TextHud.h)
  
# Prop team
SET(PROP2_PROP_TEAM_HEADERS
    ${PROP2_SRC_DIR}/PropTeam/AbstractArtDirector.h
    ${PROP2_SRC_DIR}/PropTeam/AbstractChoreographer.h
    ${PROP2_SRC_DIR}/PropTeam/AbstractPropDesigner.h
    ${PROP2_SRC_DIR}/PropTeam/AbstractPropTeam.h
    ${PROP2_SRC_DIR}/PropTeam/AbstractTeamMember.h
    ${PROP2_SRC_DIR}/PropTeam/GlArtDirector.h
    ${PROP2_SRC_DIR}/PropTeam/GlArtDirectorShaders.h
    ${PROP2_SRC_DIR}/PropTeam/StdChoreographer.h
    ${PROP2_SRC_DIR}/PropTeam/StdGlPropTeam.h
    ${PROP2_SRC_DIR}/PropTeam/StdPropDesigner.h)
    
# Shape
SET(PROP2_SHAPE_HEADERS
    ${PROP2_SRC_DIR}/Shape/AbstractShape.h
    ${PROP2_SRC_DIR}/Shape/Circle.h
    ${PROP2_SRC_DIR}/Shape/Polygon.h)

# All the header files #
SET(PROP2_HEADERS
    ${PROP2_COSTUME_HEADERS}
    ${PROP2_HUD_HEADERS}
    ${PROP2_PROP_TEAM_HEADERS}
    ${PROP2_SHAPE_HEADERS}
    ${PROP2_SRC_DIR}/AbstractProp.h  
    ${PROP2_SRC_DIR}/libPropRoom2D_global.h)
    

## Sources ##
# Costumes
SET(PROP2_COSTUME_SOURCES
    ${PROP2_SRC_DIR}/Costume/AbstractCostume.cpp
    ${PROP2_SRC_DIR}/Costume/CircleCostume.cpp
    ${PROP2_SRC_DIR}/Costume/PolygonCostume.cpp)

# Huds
SET(PROP2_HUD_SOURCES
    ${PROP2_SRC_DIR}/Hud/AbstractHud.cpp
    ${PROP2_SRC_DIR}/Hud/ImageHud.cpp
    ${PROP2_SRC_DIR}/Hud/TextHud.cpp)
    
# Prop Team
SET(PROP2_PROP_TEAM_SOURCES
    ${PROP2_SRC_DIR}/PropTeam/AbstractPropTeam.cpp
    ${PROP2_SRC_DIR}/PropTeam/GlArtDirector.cpp
    ${PROP2_SRC_DIR}/PropTeam/GlArtDirectorShaders.cpp
    ${PROP2_SRC_DIR}/PropTeam/StdChoreographer.cpp
    ${PROP2_SRC_DIR}/PropTeam/StdGlPropTeam.cpp
    ${PROP2_SRC_DIR}/PropTeam/StdPropDesigner.cpp)
    
# Shape
SET(PROP2_SHAPE_SOURCES
    ${PROP2_SRC_DIR}/Shape/AbstractShape.cpp
    ${PROP2_SRC_DIR}/Shape/Circle.cpp
    ${PROP2_SRC_DIR}/Shape/Polygon.cpp)

# All the source files #
SET(PROP2_SOURCES
    ${PROP2_COSTUME_SOURCES}
    ${PROP2_HUD_SOURCES}
    ${PROP2_PROP_TEAM_SOURCES}
    ${PROP2_SHAPE_SOURCES}
    ${PROP2_SRC_DIR}/AbstractProp.cpp)
    

## Global ##
SET(PROP2_SRC_FILES
    ${PROP2_HEADERS}
    ${PROP2_SOURCES})

SET(PROP2_CONFIG_FILES
    ${PROP2_SRC_DIR}/CMakeLists.txt
    ${PROP2_SRC_DIR}/FileLists.cmake
    ${PROP2_SRC_DIR}/LibLists.cmake)

SET(PROP2_INCLUDE_DIR
    ${PROP2_SRC_DIR})
SET(PROP2_INCLUDE_DIR
    ${PROP2_INCLUDE_DIR}
    PARENT_SCOPE)


## Source groups ##
SOURCE_GROUP("Config" FILES PROP2_CONFIG_FILES)