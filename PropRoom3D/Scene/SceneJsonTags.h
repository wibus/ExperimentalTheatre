#ifndef PROPROOM3D_SCENEJSONTAGS_H
#define PROPROOM3D_SCENEJSONTAGS_H

#include <QString>


namespace prop3
{
    // Scene object
    extern const QString SCENE_MATERIAL_ARRAY;
    extern const QString SCENE_COATING_ARRAY;
    extern const QString SCENE_SURFACE_ARRAY;
    extern const QString SCENE_PROP_ARRAY;

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

    // Materials
    extern const QString MATERIAL_TYPE;
    extern const QString MATERIAL_TYPE_AIR;
    extern const QString MATERIAL_TYPE_CONCRETE;
    extern const QString MATERIAL_TYPE_GLASS;
    extern const QString MATERIAL_TYPE_METAL;
    extern const QString MATERIAL_REFRACTIVE_INDEX;
    extern const QString MATERIAL_COLOR;
    extern const QString MATERIAL_DYE_CONCENTRATION;
    extern const QString MATERIAL_GLOSSINESS;

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
}

#endif // PROPROOM3D_SCENEJSONTAGS_H
