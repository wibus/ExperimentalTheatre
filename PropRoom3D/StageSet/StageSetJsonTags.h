#ifndef PROPROOM3D_STAGESETJSONTAGS_H
#define PROPROOM3D_STAGESETJSONTAGS_H

#include <QString>


namespace prop3
{
    // StageSet object
    extern const QString STAGESET_ENVIRONMENT_OBJECT;
    extern const QString STAGESET_BACKDROP_ARRAY;
    extern const QString STAGESET_MATERIAL_ARRAY;
    extern const QString STAGESET_COATING_ARRAY;
    extern const QString STAGESET_SURFACE_ARRAY;
    extern const QString STAGESET_PROP_ARRAY;

    // Props
    extern const QString PROP_TYPE;
    extern const QString PROP_TYPE_PROP;
    extern const QString PROP_SURFACE_ELEMENTS;
    extern const QString PROP_BOUNDING_SURFACE;

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

    // Environments
    extern const QString ENVIRONMENT_TYPE;
    extern const QString ENVIRONMENT_TYPE_ENVIRONMENT;
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
}

#endif // PROPROOM3D_STAGESETJSONTAGS_H
