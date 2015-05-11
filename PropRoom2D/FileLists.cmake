## Headers ##
# Costumes
SET(PROP2_COSTUME_HEADERS
    ${PROP2_SRC_DIR}/Prop/Costume/AbstractCostume.h
    ${PROP2_SRC_DIR}/Prop/Costume/CircleCostume.h
    ${PROP2_SRC_DIR}/Prop/Costume/PolygonCostume.h)

# Hardwares
SET(PROP2_HARDWARE_HEADERS
    ${PROP2_SRC_DIR}/Prop/Hardware/Hardware.h)

# Huds
SET(PROP2_HUD_HEADERS
    ${PROP2_SRC_DIR}/Prop/Hud/AbstractHud.h
    ${PROP2_SRC_DIR}/Prop/Hud/ImageHud.h
    ${PROP2_SRC_DIR}/Prop/Hud/TextHud.h)

# Shape
SET(PROP2_SHAPE_HEADERS
    ${PROP2_SRC_DIR}/Prop/Shape/AbstractShape.h
    ${PROP2_SRC_DIR}/Prop/Shape/Circle.h
    ${PROP2_SRC_DIR}/Prop/Shape/Polygon.h
    ${PROP2_SRC_DIR}/Prop/Shape/Segment2D.h)

# Props
SET(PROP2_PROP_HEADERS
    ${PROP2_SRC_DIR}/Prop/AbstractProp.h)

# Art Director
SET(PROP2_ART_DIRECTOR_HEADERS
    ${PROP2_SRC_DIR}/Team/ArtDirector/AbstractArtDirector.h
    ${PROP2_SRC_DIR}/Team/ArtDirector/GlArtDirector.h)

# Choreographer
SET(PROP2_CHOREOGRAPHER_HEADERS
    ${PROP2_SRC_DIR}/Team/Choreographer/AbstractChoreographer.h
    ${PROP2_SRC_DIR}/Team/Choreographer/StdChoreographer.h)

# Designer
SET(PROP2_DESIGNER_HEADERS
    ${PROP2_SRC_DIR}/Team/Designer/AbstractDesigner.h
    ${PROP2_SRC_DIR}/Team/Designer/StdDesigner.h)
  
# Prop team
SET(PROP2_TEAM_HEADERS
    ${PROP2_SRC_DIR}/Team/AbstractTeam.h
    ${PROP2_SRC_DIR}/Team/StdTeam.h)

# All the header files #
SET(PROP2_HEADERS
    ${PROP2_COSTUME_HEADERS}
    ${PROP2_HARDWARE_HEADERS}
    ${PROP2_HUD_HEADERS}
    ${PROP2_SHAPE_HEADERS}
    ${PROP2_PROP_HEADERS}
    ${PROP2_ART_DIRECTOR_HEADERS}
    ${PROP2_CHOREOGRAPHER_HEADERS}
    ${PROP2_DESIGNER_HEADERS}
    ${PROP2_TEAM_HEADERS}
    ${PROP2_SRC_DIR}/libPropRoom2D_global.h)
    

## Sources ##
# Costumes
SET(PROP2_COSTUME_SOURCES
    ${PROP2_SRC_DIR}/Prop/Costume/AbstractCostume.cpp
    ${PROP2_SRC_DIR}/Prop/Costume/CircleCostume.cpp
    ${PROP2_SRC_DIR}/Prop/Costume/PolygonCostume.cpp)

# Hardwares
SET(PROP2_HARDWARE_SOURCES
    ${PROP2_SRC_DIR}/Prop/Hardware/Hardware.cpp)

# Huds
SET(PROP2_HUD_SOURCES
    ${PROP2_SRC_DIR}/Prop/Hud/AbstractHud.cpp
    ${PROP2_SRC_DIR}/Prop/Hud/ImageHud.cpp
    ${PROP2_SRC_DIR}/Prop/Hud/TextHud.cpp)

# Shape
SET(PROP2_SHAPE_SOURCES
    ${PROP2_SRC_DIR}/Prop/Shape/AbstractShape.cpp
    ${PROP2_SRC_DIR}/Prop/Shape/Circle.cpp
    ${PROP2_SRC_DIR}/Prop/Shape/Polygon.cpp
    ${PROP2_SRC_DIR}/Prop/Shape/Segment2D.cpp)

# Props
SET(PROP2_PROP_SOURCES
    ${PROP2_SRC_DIR}/Prop/AbstractProp.cpp)

# Art Director
SET(PROP2_ART_DIRECTOR_SOURCES
    ${PROP2_SRC_DIR}/Team/ArtDirector/GlArtDirector.cpp)

# Choreographer
SET(PROP2_CHOREOGRAPHER_SOURCES
    ${PROP2_SRC_DIR}/Team/Choreographer/StdChoreographer.cpp)

# Designer
SET(PROP2_DESIGNER_SOURCES
    ${PROP2_SRC_DIR}/Team/Designer/StdDesigner.cpp)
    
# Prop Team
SET(PROP2_TEAM_SOURCES
    ${PROP2_SRC_DIR}/Team/AbstractTeam.cpp
    ${PROP2_SRC_DIR}/Team/StdTeam.cpp)

# All the source files #
SET(PROP2_SOURCES
    ${PROP2_COSTUME_SOURCES}
    ${PROP2_HARDWARE_SOURCES}
    ${PROP2_HUD_SOURCES}
    ${PROP2_SHAPE_SOURCES}
    ${PROP2_PROP_SOURCES}
    ${PROP2_ART_DIRECTOR_SOURCES}
    ${PROP2_CHOREOGRAPHER_SOURCES}
    ${PROP2_DESIGNER_SOURCES}
    ${PROP2_TEAM_SOURCES})
    

## Resources
SET(PROP2_RCC_FILES
    ${PROP2_SRC_DIR}/resources/PropRoom2D_Resources.qrc)
SET(PROP2_FONT_FILES
    ${PROP2_SRC_DIR}/resources/fonts/Arial.png
    ${PROP2_SRC_DIR}/resources/fonts/Arial.flw)
SET(PROP2_SHADER_FILES
    ${PROP2_SRC_DIR}/resources/shaders/ShapeVertex.vert
    ${PROP2_SRC_DIR}/resources/shaders/CircleFragment.frag
    ${PROP2_SRC_DIR}/resources/shaders/PolygonFragment.frag
    ${PROP2_SRC_DIR}/resources/shaders/HudVertex.vert
    ${PROP2_SRC_DIR}/resources/shaders/HudFragment.frag)
SET(PROP2_RESOURCE_FILES
    ${PROP2_RCC_FILES}
    ${PROP2_FONT_FILES}
    ${PROP2_SHADER_FILES})

QT5_ADD_RESOURCES(PROP2_RCC_SRCS ${PROP2_RCC_FILES})


## Global ##
SET(PROP2_CONFIG_FILES
    ${PROP2_SRC_DIR}/CMakeLists.txt
    ${PROP2_SRC_DIR}/FileLists.cmake
    ${PROP2_SRC_DIR}/LibLists.cmake)

SET(PROP2_SRC_FILES
    ${PROP2_HEADERS}
    ${PROP2_SOURCES}
    ${PROP2_CONFIG_FILES}
    ${PROP2_RESOURCE_FILES}
    ${PROP2_RCC_SRCS})



## Source groups ##
SOURCE_GROUP("Header Files" FILES ${PROP2_HEADERS_FILTER})
SOURCE_GROUP("Header Files\\Prop\\Costume" FILES ${PROP2_COSTUME_HEADERS})
SOURCE_GROUP("Header Files\\Prop\\Hardware" FILES ${PROP2_HARDWARE_HEADERS})
SOURCE_GROUP("Header Files\\Prop\\Hud" FILES ${PROP2_HUD_HEADERS})
SOURCE_GROUP("Header Files\\Prop\\Shape" FILES ${PROP2_SHAPE_HEADERS})
SOURCE_GROUP("Header Files\\Prop" FILES ${PROP2_PROP_HEADERS})
SOURCE_GROUP("Header Files\\Team" FILES ${PROP2_TEAM_HEADERS})

SOURCE_GROUP("Source Files" FILES ${PROP2_SOURCES_FILTER})
SOURCE_GROUP("Source Files\\Prop\\Costume" FILES ${PROP2_COSTUME_SOURCES})
SOURCE_GROUP("Source Files\\Prop\\Hardware" FILES ${PROP2_HARDWARE_SOURCES})
SOURCE_GROUP("Source Files\\Prop\\Hud" FILES ${PROP2_HUD_SOURCES})
SOURCE_GROUP("Source Files\\Prop\\Shape" FILES ${PROP2_SHAPE_SOURCES})
SOURCE_GROUP("Source Files\\Prop" FILES ${PROP2_PROP_SOURCES})
SOURCE_GROUP("Source Files\\Team" FILES ${PROP2_TEAM_SOURCES})

SOURCE_GROUP("Config" FILES ${PROP2_CONFIG_FILES})

SOURCE_GROUP("Resources" FILES ${PROP2_RCC_FILES})
SOURCE_GROUP("Resources\\Fonts" FILES ${PROP2_FONT_FILES})
SOURCE_GROUP("Resources\\Shaders" FILES ${PROP2_SHADER_FILES})
