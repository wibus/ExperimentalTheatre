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
    extern const QString PROP_MATERIAL;
    extern const QString PROP_SURFACE;
    extern const QString PROP_BOUNDING_SURFACE;

    // Surfaces
    extern const QString SURFACE_TYPE;
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
    extern const QString SURFACE_TEX_ORIGIN;
    extern const QString SURFACE_TEX_U_DIR;
    extern const QString SURFACE_TEX_V_DIR;
    extern const QString SURFACE_REPRESENTATION;
    extern const QString SURFACE_RADIUS;
    extern const QString SURFACE_CENTER;
    extern const QString SURFACE_TRANSFORM;

    // Materials
    extern const QString MATERIAL_TYPE;
    extern const QString MATERIAL_TYPE_AIR;
    extern const QString MATERIAL_TYPE_FOG;
    extern const QString MATERIAL_TYPE_CONCRETE;
    extern const QString MATERIAL_TYPE_GLASS;
    extern const QString MATERIAL_TYPE_METAL;
    extern const QString MATERIAL_REFRACTIVE_INDEX;
    extern const QString MATERIAL_COLOR;
    extern const QString MATERIAL_CONCENTRATION;
    extern const QString MATERIAL_GLOSSINESS;
    extern const QString MATERIAL_RADIUS;

    // Coatings
    extern const QString COATING_TYPE;
    extern const QString COATING_TYPE_NOCOATING;
    extern const QString COATING_TYPE_FLATPAINT;
    extern const QString COATING_TYPE_GLOSSYPAINT;
    extern const QString COATING_TYPE_TEXTUREDFLATPAINT;
    extern const QString COATING_TYPE_TEXTUREDGLOSSYPAINT;
    extern const QString COATING_COLOR;
    extern const QString COATING_GLOSSINESS;
    extern const QString COATING_DEFAULT_COLOR;
    extern const QString COATING_DEFAULT_GLOSS;
    extern const QString COATING_TEXTURE_NAME;
    extern const QString COATING_GLOSS_MAP_NAME;
    extern const QString COATING_VARNISH_REFRACTIVE_INDEX;

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
