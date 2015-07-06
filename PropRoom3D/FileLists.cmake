## Headers ##
# Art Director
SET(PROP3_ART_DIRECTOR_HEADERS
    ${PROP3_SRC_DIR}/Team/ArtDirector/AbstractArtDirector.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerClient.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerServer.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.h
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.h)

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

# Implicit Surfaces
SET(PROP3_IMPLICIT_SURFACE_HEADERS
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/ImplicitSurface.h
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Plane.h
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Quadric.h
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Sphere.h)

# Lighting
SET(PROP3_LIGHTING_HEADERS
    ${PROP3_SRC_DIR}/Prop/Lighting/Light3D.h)

# Materials
SET(PROP3_MATERIAL_HEADERS
    ${PROP3_SRC_DIR}/Prop/Material/Material.h
    ${PROP3_SRC_DIR}/Prop/Material/Air.h
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

# Prop
SET(PROP3_PROP_HEADERS
    ${PROP3_COATING_HEADERS}
    ${PROP3_IMPLICIT_SURFACE_HEADERS}
    ${PROP3_HARDWARE_HEADERS}
    ${PROP3_LIGHTING_HEADERS}
    ${PROP3_MATERIAL_HEADERS}
    ${PROP3_RAY_HEADERS}
    ${PROP3_SRC_DIR}/Prop/Prop.h)

# Scene
SET(PROP3_SCENE_HEADERS
    ${PROP3_SRC_DIR}/Scene/Scene.h
    ${PROP3_SRC_DIR}/Scene/SceneReader.h
    ${PROP3_SRC_DIR}/Scene/SceneWriter.h)


# All the header files #
SET(PROP3_HEADERS
    ${PROP3_TEAM_HEADERS}
    ${PROP3_PROP_HEADERS}
    ${PROP3_SCENE_HEADERS}
    ${PROP3_SRC_DIR}/libPropRoom3D_global.h)
    

## Sources ##
# Art Director
SET(PROP3_ART_DIRECTOR_SOURCES
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerClient.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerServer.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/CpuRaytracerWorker.cpp
    ${PROP3_SRC_DIR}/Team/ArtDirector/GlPostProdUnit.cpp)

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

# Implicit Surfaces
SET(PROP3_IMPLICIT_SURFACE_SOURCES
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/ImplicitSurface.cpp
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Plane.cpp
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Quadric.cpp
    ${PROP3_SRC_DIR}/Prop/ImplicitSurface/Sphere.cpp)

# Lighting
SET(PROP3_LIGHTING_SOURCES
    ${PROP3_SRC_DIR}/Prop/Lighting/Light3D.cpp)

# Materials
SET(PROP3_MATERIAL_SOURCES
    ${PROP3_SRC_DIR}/Prop/Material/Material.cpp
    ${PROP3_SRC_DIR}/Prop/Material/Air.cpp
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

# Props
SET(PROP3_PROP_SOURCES
    ${PROP3_COATING_SOURCES}
    ${PROP3_IMPLICIT_SURFACE_SOURCES}
    ${PROP3_HARDWARE_SOURCES}
    ${PROP3_LIGHTING_SOURCES}
    ${PROP3_MATERIAL_SOURCES}
    ${PROP3_RAY_SOURCES}
    ${PROP3_SRC_DIR}/Prop/Prop.cpp)

# Scene
SET(PROP3_SCENE_SOURCES
    ${PROP3_SRC_DIR}/Scene/Scene.cpp
    ${PROP3_SRC_DIR}/Scene/SceneReader.cpp
    ${PROP3_SRC_DIR}/Scene/SceneWriter.cpp)

# All the source files #
SET(PROP3_SOURCES
    ${PROP3_TEAM_SOURCES}
    ${PROP3_PROP_SOURCES}
    ${PROP3_SCENE_SOURCES})

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
