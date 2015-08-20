#include "SceneJson.h"


namespace prop3
{
    // Scene object
    const QString SCENE_MATERIAL_ARRAY  = "Materials";
    const QString SCENE_COATING_ARRAY   = "Coatings";
    const QString SCENE_SURFACE_ARRAY   = "Surfaces";
    const QString SCENE_PROP_ARRAY      = "Props";

    // Props
    const QString PROP_TYPE             = "Type";
    const QString PROP_TYPE_PROP        = "Prop";
    const QString PROP_MATERIAL         = "Material";
    const QString PROP_SURFACE          = "Surface";
    const QString PROP_BOUNDING_SURFACE = "Bounding Surface";

    // Surfaces
    const QString SURFACE_TYPE              = "Type";
    const QString SURFACE_TYPE_PLANE        = "Plane";
    const QString SURFACE_TYPE_PLANETEXTURE = "Plane Texture";
    const QString SURFACE_TYPE_QUADRIC      = "Quadric";
    const QString SURFACE_TYPE_SPHERE       = "Sphere";
    const QString SURFACE_OPERATOR_GHOST    = "~";
    const QString SURFACE_OPERATOR_INVERSE  = "!";
    const QString SURFACE_OPERATOR_OR       = "|";
    const QString SURFACE_OPERATOR_AND      = "&";
    const QString SURFACE_DISTANCE          = "Distance";
    const QString SURFACE_NORMAL            = "Normal";
    const QString SURFACE_COATING           = "Coating";
    const QString SURFACE_TEX_ORIGIN        = "Tex Origin";
    const QString SURFACE_TEX_U_DIR         = "Tex U Dir";
    const QString SURFACE_TEX_V_DIR         = "Tex V Dir";
    const QString SURFACE_REPRESENTATION    = "Representation";
    const QString SURFACE_RADIUS            = "Radius";
    const QString SURFACE_CENTER            = "Center";

    // Materials
    const QString MATERIAL_TYPE                 = "Type";
    const QString MATERIAL_TYPE_AIR             = "Air";
    const QString MATERIAL_TYPE_CONCRETE        = "Concrete";
    const QString MATERIAL_TYPE_GLASS           = "Glass";
    const QString MATERIAL_TYPE_METAL           = "Metal";
    const QString MATERIAL_REFRACTIVE_INDEX     = "Refractive Index";
    const QString MATERIAL_COLOR                = "Color";
    const QString MATERIAL_DYE_CONCENTRATION    = "Dyr Concentration";
    const QString MATERIAL_GLOSSINESS           = "Glossiness";

    // Coatings
    const QString COATING_TYPE                      = "Type";
    const QString COATING_TYPE_NOCOATING            = "NoCoating";
    const QString COATING_TYPE_FLATPAINT            = "FlatPaint";
    const QString COATING_TYPE_GLOSSYPAINT          = "GlossyPaint";
    const QString COATING_TYPE_TEXTUREDFLATPAINT    = "TexturedFlatPaint";
    const QString COATING_TYPE_TEXTUREDGLOSSYPAINT  = "TexturedGlossyPaint";
    const QString COATING_COLOR                     = "Color";
    const QString COATING_GLOSSINESS                = "Glossiness";
    const QString COATING_DEFAULT_COLOR             = "Default Color";
    const QString COATING_DEFAULT_GLOSS             = "Default Gloss";
    const QString COATING_TEXTURE_NAME              = "Tex Name";
    const QString COATING_GLOSS_MAP_NAME            = "Gloss Name";
    const QString COATING_VARNISH_REFRACTIVE_INDEX  = "Varnish Refractive Index";
}
