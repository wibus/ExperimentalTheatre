#ifndef PROPROOM3D_STAGESETJSONTAGS_H
#define PROPROOM3D_STAGESETJSONTAGS_H

#include <QString>


namespace prop3
{
    // Document object
    extern const QString DOCUMENT_MATERIAL_ARRAY;
    extern const QString DOCUMENT_COATING_ARRAY;
    extern const QString DOCUMENT_SURFACE_ARRAY;
    extern const QString DOCUMENT_SAMPLER_ARRAY;
    extern const QString DOCUMENT_LIGHTS_ARRAY;
    extern const QString DOCUMENT_STAGE_SET;

    // Environments
    extern const QString ENVIRONMENT_BACKDROP;
    extern const QString ENVIRONMENT_AMBIENT_MATERIAL;

    // Backdrops
    extern const QString BACKDROP_TYPE;
    extern const QString BACKDROP_TYPE_PROCEDURALSUN;
    extern const QString BACKDROP_IS_DIRECTLY_VISIBLE;
    extern const QString BACKDROP_SUN_COLOR;
    extern const QString BACKDROP_SKY_COLOR;
    extern const QString BACKDROP_SKYLINE_COLOR;
    extern const QString BACKDROP_GROUND_COLOR;
    extern const QString BACKDROP_GROUND_HEIGHT;
    extern const QString BACKDROP_SUN_DIR;

    // Lights
    extern const QString LIGHT_TYPE;
    extern const QString LIGHT_TYPE_BULB;
    extern const QString LIGHT_RADIANT_FLUX;
    extern const QString LIGHT_AREA_SAMPLER;
    extern const QString LIGHT_COATING;
    extern const QString LIGHT_IS_ON;

    // Area Samplers
    extern const QString SAMPLER_TYPE;
    extern const QString SAMPLER_TYPE_CIRCULAR;
    extern const QString SAMPLER_TYPE_SPHERICAL;
    extern const QString SAMPLER_TWO_SIDED;
    extern const QString SAMPLER_CENTER;
    extern const QString SAMPLER_NORMAL;
    extern const QString SAMPLER_RADIUS;


    // Handle nodes
    extern const QString HANDLE_NAME;
    extern const QString HANDLE_IS_VISIBLE;

    // Zones
    extern const QString ZONE_BOUNDS;
    extern const QString ZONE_PROPS;
    extern const QString ZONE_LIGHTS;
    extern const QString ZONE_SUBZONES;

    // Stage set
    extern const QString STAGESET_ENVIRONMENT;


    // Props
    extern const QString PROP_SURFACES;

    // Surfaces
    extern const QString SURFACE_TYPE;
    extern const QString SURFACE_TYPE_BOX;
    extern const QString SURFACE_TYPE_BOX_TEXTURE;
    extern const QString SURFACE_TYPE_PLANE;
    extern const QString SURFACE_TYPE_PLANETEXTURE;
    extern const QString SURFACE_TYPE_QUADRIC;
    extern const QString SURFACE_TYPE_SPHERE;
    extern const QString SURFACE_OPERATOR_SHELL;
    extern const QString SURFACE_OPERATOR_GHOST;
    extern const QString SURFACE_OPERATOR_INVERSE;
    extern const QString SURFACE_OPERATOR_OR;
    extern const QString SURFACE_OPERATOR_AND;
    extern const QString SURFACE_DISTANCE;
    extern const QString SURFACE_NORMAL;
    extern const QString SURFACE_COATING;
    extern const QString SURFACE_INNER_MATERIAL;
    extern const QString SURFACE_OUTER_MATERIAL;
    extern const QString SURFACE_TEX_ORIGIN;
    extern const QString SURFACE_TEX_U_DIR;
    extern const QString SURFACE_TEX_V_DIR;
    extern const QString SURFACE_TEX_MAIN_SIDE_ONLY;
    extern const QString SURFACE_REPRESENTATION;
    extern const QString SURFACE_RADIUS;
    extern const QString SURFACE_CENTER;
    extern const QString SURFACE_MIN_CORNER;
    extern const QString SURFACE_MAX_CORNER;
    extern const QString SURFACE_TRANSFORM;

    // Materials
    extern const QString MATERIAL_TYPE;
    extern const QString MATERIAL_TYPE_UNIFORMSTD;
    extern const QString MATERIAL_OPACITY;
    extern const QString MATERIAL_CONDUCTIVITY;
    extern const QString MATERIAL_REFRACTIVE_INDEX;
    extern const QString MATERIAL_SCATTERING;
    extern const QString MATERIAL_COLOR;

    // Coatings
    extern const QString COATING_TYPE;
    extern const QString COATING_TYPE_EMISSIVE;
    extern const QString COATING_TYPE_UNIFORMSTD;
    extern const QString COATING_TYPE_TEXTUREDSTD;
    extern const QString COATING_ROUGHNESS;
    extern const QString COATING_PAINT_COLOR;
    extern const QString COATING_PAINT_REFRACTIVE_INDEX;
    extern const QString COATING_DEFAULT_ROUGHNESS;
    extern const QString COATING_DEFAULT_PAINT_COLOR;
    extern const QString COATING_ROUGHNESS_TEX_NAME;
    extern const QString COATING_PAINT_COLOR_TEX_NAME;
    extern const QString COATING_TEXTURE_FILTER;
    extern const QString COATING_TEXTURE_WRAPPER;
    extern const QString COATING_EMITTED_RADIANCE;
}

#endif // PROPROOM3D_STAGESETJSONTAGS_H
