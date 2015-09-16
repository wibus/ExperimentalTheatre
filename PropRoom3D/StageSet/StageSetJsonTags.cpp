#include "StageSetJsonTags.h"


namespace prop3
{
    // StageSet object
    const QString STAGESET_ENVIRONMENT_OBJECT = "Environment";
    const QString STAGESET_BACKDROP_ARRAY     = "Backdrops";
    const QString STAGESET_MATERIAL_ARRAY     = "Materials";
    const QString STAGESET_COATING_ARRAY      = "Coatings";
    const QString STAGESET_SURFACE_ARRAY      = "Surfaces";
    const QString STAGESET_PROP_ARRAY         = "Props";

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
    const QString SURFACE_OPERATOR_SHELL    = "Shell()";
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
    const QString SURFACE_TRANSFORM         = "Transform";

    // Materials
    const QString MATERIAL_TYPE                 = "Type";
    const QString MATERIAL_TYPE_AIR             = "Air";
    const QString MATERIAL_TYPE_FOG             = "Fog";
    const QString MATERIAL_TYPE_CONCRETE        = "Concrete";
    const QString MATERIAL_TYPE_GLASS           = "Glass";
    const QString MATERIAL_TYPE_METAL           = "Metal";
    const QString MATERIAL_REFRACTIVE_INDEX     = "Refractive Index";
    const QString MATERIAL_COLOR                = "Color";
    const QString MATERIAL_CONCENTRATION        = "Concentration";
    const QString MATERIAL_GLOSSINESS           = "Glossiness";
    const QString MATERIAL_RADIUS               = "Radius";

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

    // Environments
    const QString ENVIRONMENT_TYPE                  = "Type";
    const QString ENVIRONMENT_TYPE_ENVIRONMENT      = "Environment";
    const QString ENVIRONMENT_BACKDROP              = "Backdrop";
    const QString ENVIRONMENT_AMBIENT_MATERIAL      = "Ambient Material";

    // Backdrops
    const QString BACKDROP_TYPE                 = "Type";
    const QString BACKDROP_TYPE_PROCEDURALSUN   = "ProceduralSun";
    const QString BACKDROP_IS_DIRECTLY_VISIBLE  = "Is Directly Visible";
    const QString BACKDROP_SUN_COLOR            = "Sun Color";
    const QString BACKDROP_SKY_COLOR            = "Sky Color";
    const QString BACKDROP_SKYLINE_COLOR        = "Skyline Color";
    const QString BACKDROP_GROUND_COLOR         = "Ground Color";
    const QString BACKDROP_GROUND_HEIGHT        = "Ground Height";
    const QString BACKDROP_SUN_DIR              = "Sun Dir";
}
