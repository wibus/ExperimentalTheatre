## Headers ##

# Backdrops
SET(PROP3_BACKDROP_HEADERS
    ${PROP3_SRC_DIR}/Node/Light/Backdrop/Backdrop.h
    ${PROP3_SRC_DIR}/Node/Light/Backdrop/ProceduralSun.h)

# Light Bulbs
SET(PROP3_LIGHTBULB_HEADERS
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/LightBulb.h
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/CircularLight.h
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/SphericalLight.h)

# Light
SET(PROP3_LIGHT_HEADERS
    ${PROP3_BACKDROP_HEADERS}
    ${PROP3_LIGHTBULB_HEADERS}
    ${PROP3_SRC_DIR}/Node/Light/LightCast.h
    ${PROP3_SRC_DIR}/Node/Light/LightUtils.h)

# Debug
SET(PROP3_DEBUG_HEADERS
    ${PROP3_SRC_DIR}/Node/Debug/DebugLineStrip.h
    ${PROP3_SRC_DIR}/Node/Debug/DebugPointCloud.h)

# Coatings
SET(PROP3_COATING_HEADERS
    ${PROP3_SRC_DIR}/Node/Prop/Coating/Coating.h
    ${PROP3_SRC_DIR}/Node/Prop/Coating/StdCoating.h
    ${PROP3_SRC_DIR}/Node/Prop/Coating/EmissiveCoating.h
    ${PROP3_SRC_DIR}/Node/Prop/Coating/UniformStdCoating.h
    ${PROP3_SRC_DIR}/Node/Prop/Coating/TexturedStdCoating.h)

# Materials
SET(PROP3_MATERIAL_HEADERS
    ${PROP3_SRC_DIR}/Node/Prop/Material/Material.h
    ${PROP3_SRC_DIR}/Node/Prop/Material/StdMaterial.h
    ${PROP3_SRC_DIR}/Node/Prop/Material/UniformStdMaterial.h)

# Surfaces
SET(PROP3_SURFACE_HEADERS
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Surface.h
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Box.h
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Disk.h
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Plane.h
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Quadric.h
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Sphere.h)

# Prop
SET(PROP3_PROP_HEADERS
    ${PROP3_COATING_HEADERS}
    ${PROP3_MATERIAL_HEADERS}
    ${PROP3_SURFACE_HEADERS}
    ${PROP3_SRC_DIR}/Node/Prop/Prop.h)

# Nodes
SET(PROP3_NODE_HEADERS
    ${PROP3_LIGHT_HEADERS}
    ${PROP3_DEBUG_HEADERS}
    ${PROP3_PROP_HEADERS}
    ${PROP3_SRC_DIR}/Node/Node.h
    ${PROP3_SRC_DIR}/Node/Visitor.h
    ${PROP3_SRC_DIR}/Node/HandleNode.h
    ${PROP3_SRC_DIR}/Node/StageZone.h
    ${PROP3_SRC_DIR}/Node/StageSet.h)

# Rays
SET(PROP3_RAY_HEADERS
    ${PROP3_SRC_DIR}/Ray/Raycast.h
    ${PROP3_SRC_DIR}/Ray/RayHitList.h
    ${PROP3_SRC_DIR}/Ray/RayHitReport.h)

# Serialization
SET(PROP3_SERIAL_HEADERS
    ${PROP3_SRC_DIR}/Serial/JsonTags.h
    ${PROP3_SRC_DIR}/Serial/JsonReader.h
    ${PROP3_SRC_DIR}/Serial/JsonWriter.h)

# Films
SET(PROP3_FILM_HEADERS
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/Film.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/Tile.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/ConvergentFilm.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/NetworkFilm.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/StaticFilm.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/PixelPrioritizer.h)

# Network
SET(PROP3_NETWORK_HEADERS
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/TcpServer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/ServerSocket.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/TileMessage.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/UpdateMessage.h)

# Art Director
SET(PROP3_ART_DIRECTOR_HEADERS
    ${PROP3_FILM_HEADERS}
    ${PROP3_NETWORK_HEADERS}
    ${PROP3_SRC_DIR}/Team/ArtDirector/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorDummy.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorClient.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorServer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/DebugRenderer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerEngine.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/RaytracerState.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/SearchStructure.h)

# Choreographer
SET(PROP3_CHOREOGRAPHER_HEADERS
    ${PROP3_SRC_DIR}/Team/Choreographer/AbstractChoreographer.h
    ${PROP3_SRC_DIR}/Team/Choreographer/StdChoreographer.h)

# Team
SET(PROP3_TEAM_HEADERS
    ${PROP3_ART_DIRECTOR_HEADERS}
    ${PROP3_CHOREOGRAPHER_HEADERS}
    ${PROP3_SRC_DIR}/Team/AbstractTeam.h
    ${PROP3_SRC_DIR}/Team/DummyTeam.h
    ${PROP3_SRC_DIR}/Team/StdTeam.h)


# All the header files #
SET(PROP3_HEADERS
    ${PROP3_NODE_HEADERS}
    ${PROP3_RAY_HEADERS}
    ${PROP3_SERIAL_HEADERS}
    ${PROP3_TEAM_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##

# Backdrops
SET(PROP3_BACKDROP_SOURCES
    ${PROP3_SRC_DIR}/Node/Light/Backdrop/Backdrop.cpp
    ${PROP3_SRC_DIR}/Node/Light/Backdrop/ProceduralSun.cpp)

# Light Bulbs
SET(PROP3_LIGHTBULB_SOURCES
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/LightBulb.cpp
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/CircularLight.cpp
    ${PROP3_SRC_DIR}/Node/Light/LightBulb/SphericalLight.cpp)

# Light
SET(PROP3_LIGHT_SOURCES
    ${PROP3_BACKDROP_SOURCES}
    ${PROP3_LIGHTBULB_SOURCES}
    ${PROP3_SRC_DIR}/Node/Light/LightCast.cpp
    ${PROP3_SRC_DIR}/Node/Light/LightUtils.cpp)

# Debug
SET(PROP3_DEBUG_SOURCES
    ${PROP3_SRC_DIR}/Node/Debug/DebugLineStrip.cpp
    ${PROP3_SRC_DIR}/Node/Debug/DebugPointCloud.cpp)

# Coatings
SET(PROP3_COATING_SOURCES
    ${PROP3_SRC_DIR}/Node/Prop/Coating/Coating.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Coating/StdCoating.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Coating/EmissiveCoating.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Coating/UniformStdCoating.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Coating/TexturedStdCoating.cpp)

# Materials
SET(PROP3_MATERIAL_SOURCES
    ${PROP3_SRC_DIR}/Node/Prop/Material/Material.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Material/StdMaterial.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Material/UniformStdMaterial.cpp)

# Surfaces
SET(PROP3_SURFACE_SOURCES
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Surface.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Box.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Disk.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Plane.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Quadric.cpp
    ${PROP3_SRC_DIR}/Node/Prop/Surface/Sphere.cpp)

# Props
SET(PROP3_PROP_SOURCES
    ${PROP3_COATING_SOURCES}
    ${PROP3_MATERIAL_SOURCES}
    ${PROP3_SURFACE_SOURCES}
    ${PROP3_SRC_DIR}/Node/Prop/Prop.cpp)

# Nodes
SET(PROP3_NODE_SOURCES
    ${PROP3_LIGHT_SOURCES}
    ${PROP3_DEBUG_SOURCES}
    ${PROP3_PROP_SOURCES}
    ${PROP3_SRC_DIR}/Node/Node.cpp
    ${PROP3_SRC_DIR}/Node/Visitor.cpp
    ${PROP3_SRC_DIR}/Node/HandleNode.cpp
    ${PROP3_SRC_DIR}/Node/StageZone.cpp
    ${PROP3_SRC_DIR}/Node/StageSet.cpp)

# Rays
SET(PROP3_RAY_SOURCES
    ${PROP3_SRC_DIR}/Ray/Raycast.cpp
    ${PROP3_SRC_DIR}/Ray/RayHitList.cpp
    ${PROP3_SRC_DIR}/Ray/RayHitReport.cpp)

# Serialization
SET(PROP3_SERIAL_SOURCES
    ${PROP3_SRC_DIR}/Serial/JsonTags.cpp
    ${PROP3_SRC_DIR}/Serial/JsonReader.cpp
    ${PROP3_SRC_DIR}/Serial/JsonWriter.cpp)

# Films
SET(PROP3_FILM_SOURCES
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/Film.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/Tile.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/NetworkFilm.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/ConvergentFilm.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/StaticFilm.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Film/PixelPrioritizer.cpp)

# Network
SET(PROP3_NETWORK_SOURCES
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/TcpServer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/ServerSocket.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/TileMessage.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/Network/UpdateMessage.cpp)

# Art Director
SET(PROP3_ART_DIRECTOR_SOURCES
    ${PROP3_FILM_SOURCES}
    ${PROP3_NETWORK_SOURCES}
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorDummy.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorClient.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorServer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/DebugRenderer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerEngine.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/RaytracerState.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/SearchStructure.cpp)

# Choreographer
SET(PROP3_CHOREOGRAPHER_SOURCES
    ${PROP3_SRC_DIR}/Team/Choreographer/StdChoreographer.cpp)

# Team
SET(PROP3_TEAM_SOURCES
    ${PROP3_ART_DIRECTOR_SOURCES}
    ${PROP3_CHOREOGRAPHER_SOURCES}
    ${PROP3_SRC_DIR}/Team/AbstractTeam.cpp
    ${PROP3_SRC_DIR}/Team/DummyTeam.cpp
    ${PROP3_SRC_DIR}/Team/StdTeam.cpp)

# All the source files #
SET(PROP3_SOURCES
    ${PROP3_NODE_SOURCES}
    ${PROP3_RAY_SOURCES}
    ${PROP3_SERIAL_SOURCES}
    ${PROP3_TEAM_SOURCES})


## Resources
SET(PROP3_RCC_FILES
    ${PROP3_SRC_DIR}/resources/PropRoom3D_Resources.qrc)
SET(PROP3_SHADER_FILES
    ${PROP3_SRC_DIR}/resources/shaders/clip_space.vert
    ${PROP3_SRC_DIR}/resources/shaders/debugLine.vert
    ${PROP3_SRC_DIR}/resources/shaders/debugLine.frag
    ${PROP3_SRC_DIR}/resources/shaders/debugPoint.vert
    ${PROP3_SRC_DIR}/resources/shaders/debugPoint.frag
    ${PROP3_SRC_DIR}/resources/shaders/post_prod_gl130.frag
    ${PROP3_SRC_DIR}/resources/shaders/post_prod_gl440.frag)
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
