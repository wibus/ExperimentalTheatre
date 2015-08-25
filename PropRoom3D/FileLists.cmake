## Headers ##
# Art Director
SET(PROP3_ART_DIRECTOR_HEADERS
    ${PROP3_SRC_DIR}/Team/ArtDirector/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorClient.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorServer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerEngine.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/RaytracerState.h)

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
    ${PROP3_SRC_DIR}/Team/StdTeam.h)

# Coatings
SET(PROP3_COATING_HEADERS
    ${PROP3_SRC_DIR}/Prop/Coating/Coating.h
    ${PROP3_SRC_DIR}/Prop/Coating/NoCoating.h
    ${PROP3_SRC_DIR}/Prop/Coating/FlatPaint.h
    ${PROP3_SRC_DIR}/Prop/Coating/GlossyPaint.h
    ${PROP3_SRC_DIR}/Prop/Coating/TexturedFlatPaint.h
    ${PROP3_SRC_DIR}/Prop/Coating/TexturedGlossyPaint.h)

# Lighting
SET(PROP3_LIGHTING_HEADERS
    ${PROP3_SRC_DIR}/Prop/Lighting/Light3D.h)

# Materials
SET(PROP3_MATERIAL_HEADERS
    ${PROP3_SRC_DIR}/Prop/Material/Material.h
    ${PROP3_SRC_DIR}/Prop/Material/Air.h
    ${PROP3_SRC_DIR}/Prop/Material/Fog.h
    ${PROP3_SRC_DIR}/Prop/Material/Metal.h
    ${PROP3_SRC_DIR}/Prop/Material/Concrete.h
    ${PROP3_SRC_DIR}/Prop/Material/Glass.h)

# Rays
SET(PROP3_RAY_HEADERS
    ${PROP3_SRC_DIR}/Prop/Ray/Ray.h
    ${PROP3_SRC_DIR}/Prop/Ray/Raycast.h
    ${PROP3_SRC_DIR}/Prop/Ray/RayHitList.h
    ${PROP3_SRC_DIR}/Prop/Ray/RayHitReport.h
    ${PROP3_SRC_DIR}/Prop/Ray/RayUtils.h)

# Surfaces
SET(PROP3_SURFACE_HEADERS
    ${PROP3_SRC_DIR}/Prop/Surface/Surface.h
    ${PROP3_SRC_DIR}/Prop/Surface/Plane.h
    ${PROP3_SRC_DIR}/Prop/Surface/Quadric.h
    ${PROP3_SRC_DIR}/Prop/Surface/Sphere.h)

# Prop
SET(PROP3_PROP_HEADERS
    ${PROP3_COATING_HEADERS}
    ${PROP3_HARDWARE_HEADERS}
    ${PROP3_LIGHTING_HEADERS}
    ${PROP3_MATERIAL_HEADERS}
    ${PROP3_RAY_HEADERS}
    ${PROP3_SURFACE_HEADERS}
    ${PROP3_SRC_DIR}/Prop/Prop.h)

# Scene
SET(PROP3_SCENE_HEADERS
    ${PROP3_SRC_DIR}/Scene/Scene.h
    ${PROP3_SRC_DIR}/Scene/SceneNode.h
    ${PROP3_SRC_DIR}/Scene/SceneVisitor.h
    ${PROP3_SRC_DIR}/Scene/SceneJsonTags.h
    ${PROP3_SRC_DIR}/Scene/SceneJsonReader.h
    ${PROP3_SRC_DIR}/Scene/SceneJsonWriter.h)

# Backdrops
SET(PROP3_BACKDROP_HEADERS
    ${PROP3_SRC_DIR}/Environment/Backdrop/Backdrop.h
    ${PROP3_SRC_DIR}/Environment/Backdrop/ProceduralSun.h)

# Environments
SET(PROP3_ENVIRONMENT_HEADERS
    ${PROP3_BACKDROP_HEADERS}
    ${PROP3_SRC_DIR}/Environment/Environment.h)


# All the header files #
SET(PROP3_HEADERS
    ${PROP3_TEAM_HEADERS}
    ${PROP3_PROP_HEADERS}
    ${PROP3_SCENE_HEADERS}
    ${PROP3_ENVIRONMENT_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Art Director
SET(PROP3_ART_DIRECTOR_SOURCES
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorClient.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/ArtDirectorServer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerEngine.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/RaytracerState.cpp)

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
    ${PROP3_SRC_DIR}/Team/StdTeam.cpp)

# Coatings
SET(PROP3_COATING_SOURCES
    ${PROP3_SRC_DIR}/Prop/Coating/Coating.cpp
    ${PROP3_SRC_DIR}/Prop/Coating/NoCoating.cpp
    ${PROP3_SRC_DIR}/Prop/Coating/FlatPaint.cpp
    ${PROP3_SRC_DIR}/Prop/Coating/GlossyPaint.cpp
    ${PROP3_SRC_DIR}/Prop/Coating/TexturedFlatPaint.cpp
    ${PROP3_SRC_DIR}/Prop/Coating/TexturedGlossyPaint.cpp)

# Environments
SET(PROP3_ENVIRONMENT_SOURCES
    ${PROP3_SRC_DIR}/Prop/Environment/Environment.cpp
    ${PROP3_SRC_DIR}/Prop/Environment/ProceduralSun.cpp)

# Lighting
SET(PROP3_LIGHTING_SOURCES
    ${PROP3_SRC_DIR}/Prop/Lighting/Light3D.cpp)

# Materials
SET(PROP3_MATERIAL_SOURCES
    ${PROP3_SRC_DIR}/Prop/Material/Material.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Air.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Fog.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Metal.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Concrete.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Glass.cpp)

# Rays
SET(PROP3_RAY_SOURCES
    ${PROP3_SRC_DIR}/Prop/Ray/Ray.cpp
    ${PROP3_SRC_DIR}/Prop/Ray/Raycast.cpp
    ${PROP3_SRC_DIR}/Prop/Ray/RayHitList.cpp
    ${PROP3_SRC_DIR}/Prop/Ray/RayHitReport.cpp
    ${PROP3_SRC_DIR}/Prop/Ray/RayUtils.cpp)

# Surfaces
SET(PROP3_SURFACE_SOURCES
    ${PROP3_SRC_DIR}/Prop/Surface/Surface.cpp
    ${PROP3_SRC_DIR}/Prop/Surface/Plane.cpp
    ${PROP3_SRC_DIR}/Prop/Surface/Quadric.cpp
    ${PROP3_SRC_DIR}/Prop/Surface/Sphere.cpp)

# Props
SET(PROP3_PROP_SOURCES
    ${PROP3_COATING_SOURCES}
    ${PROP3_HARDWARE_SOURCES}
    ${PROP3_LIGHTING_SOURCES}
    ${PROP3_MATERIAL_SOURCES}
    ${PROP3_RAY_SOURCES}
    ${PROP3_SURFACE_SOURCES}
    ${PROP3_SRC_DIR}/Prop/Prop.cpp)

# Scene
SET(PROP3_SCENE_SOURCES
    ${PROP3_SRC_DIR}/Scene/Scene.cpp
    ${PROP3_SRC_DIR}/Scene/SceneNode.cpp
    ${PROP3_SRC_DIR}/Scene/SceneVisitor.cpp
    ${PROP3_SRC_DIR}/Scene/SceneJsonTags.cpp
    ${PROP3_SRC_DIR}/Scene/SceneJsonReader.cpp
    ${PROP3_SRC_DIR}/Scene/SceneJsonWriter.cpp)

# Backdrops
SET(PROP3_BACKDROP_SOURCES
    ${PROP3_SRC_DIR}/Environment/Backdrop/Backdrop.cpp
    ${PROP3_SRC_DIR}/Environment/Backdrop/ProceduralSun.cpp)

# Environments
SET(PROP3_ENVIRONMENT_SOURCES
    ${PROP3_BACKDROP_SOURCES}
    ${PROP3_SRC_DIR}/Environment/Environment.cpp)


# All the source files #
SET(PROP3_SOURCES
    ${PROP3_TEAM_SOURCES}
    ${PROP3_PROP_SOURCES}
    ${PROP3_SCENE_SOURCES}
    ${PROP3_ENVIRONMENT_SOURCES})

## Resources
SET(PROP3_RCC_FILES
    ${PROP3_SRC_DIR}/resources/PropRoom3D_Resources.qrc)
SET(PROP3_SHADER_FILES
    ${PROP3_SRC_DIR}/resources/shaders/clip_space.vert
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
