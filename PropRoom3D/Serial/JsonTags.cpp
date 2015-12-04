#include "JsonTags.h"


namespace prop3
{
    // StageSet object
    const QString DOCUMENT_MATERIAL_ARRAY        = "Materials";
    const QString DOCUMENT_COATING_ARRAY         = "Coatings";
    const QString DOCUMENT_SURFACE_ARRAY         = "Surfaces";
    const QString DOCUMENT_STAGE_SET             = "Stage set";

    // Environments
    const QString ENVIRONMENT_BACKDROP              = "Backdrop";
    const QString ENVIRONMENT_AMBIENT_MATERIAL      = "Ambient Material";

    // Backdrops
    const QString BACKDROP_TYPE                     = "Type";
    const QString BACKDROP_TYPE_PROCEDURALSUN       = "ProceduralSun";
    const QString BACKDROP_IS_DIRECTLY_VISIBLE      = "Is Directly Visible";
    const QString BACKDROP_SUN_COLOR                = "Sun Color";
    const QString BACKDROP_SKY_COLOR                = "Sky Color";
    const QString BACKDROP_SKYLINE_COLOR            = "Skyline Color";
    const QString BACKDROP_GROUND_COLOR             = "Ground Color";
    const QString BACKDROP_GROUND_HEIGHT            = "Ground Height";
    const QString BACKDROP_SUN_DIR                  = "Sun Dir";

    // Handle nodes
    const QString HANDLE_NAME                       = "Name";
    const QString HANDLE_IS_VISIBLE                 = "Is Visible";

    // Zones
    const QString ZONE_BOUNDS                       = "Bounds";
    const QString ZONE_PROPS                        = "Props";
    const QString ZONE_SUBZONES                     = "Subzones";

    // Stage set
    const QString STAGESET_ENVIRONMENT              = "Environment";


    // Props
    const QString PROP_SURFACES                     = "Surfaces";

    // Surfaces
    const QString SURFACE_TYPE                      = "Type";
    const QString SURFACE_TYPE_BOX                  = "Box";
    const QString SURFACE_TYPE_BOX_TEXTURE          = "Box Texture";
    const QString SURFACE_TYPE_PLANE                = "Plane";
    const QString SURFACE_TYPE_PLANETEXTURE         = "Plane Texture";
    const QString SURFACE_TYPE_QUADRIC              = "Quadric";
    const QString SURFACE_TYPE_SPHERE               = "Sphere";
    const QString SURFACE_OPERATOR_SHELL            = "Shell()";
    const QString SURFACE_OPERATOR_GHOST            = "~";
    const QString SURFACE_OPERATOR_INVERSE          = "!";
    const QString SURFACE_OPERATOR_OR               = "|";
    const QString SURFACE_OPERATOR_AND              = "&";
    const QString SURFACE_DISTANCE                  = "Distance";
    const QString SURFACE_NORMAL                    = "Normal";
    const QString SURFACE_COATING                   = "Coating";
    const QString SURFACE_INNER_MATERIAL            = "Inner Material";
    const QString SURFACE_OUTER_MATERIAL            = "Outer Material";
    const QString SURFACE_TEX_ORIGIN                = "Tex Origin";
    const QString SURFACE_TEX_U_DIR                 = "Tex U Dir";
    const QString SURFACE_TEX_V_DIR                 = "Tex V Dir";
    const QString SURFACE_TEX_MAIN_SIDE_ONLY        = "Tex Main Side Only";
    const QString SURFACE_REPRESENTATION            = "Representation";
    const QString SURFACE_RADIUS                    = "Radius";
    const QString SURFACE_CENTER                    = "Center";
    const QString SURFACE_MIN_CORNER                = "Min Corner";
    const QString SURFACE_MAX_CORNER                = "Max Corner";
    const QString SURFACE_TRANSFORM                 = "Transform";

    // Materials
    const QString MATERIAL_TYPE                     = "Type";
    const QString MATERIAL_TYPE_UNIFORMSTD          = "Uniform Std";
    const QString MATERIAL_OPACITY                  = "Opacity";
    const QString MATERIAL_CONDUCTIVITY             = "Conductivity";
    const QString MATERIAL_REFRACTIVE_INDEX         = "Refractive Index";
    const QString MATERIAL_SCATTERING               = "Scattering";
    const QString MATERIAL_COLOR                    = "Color";

    // Coatings
    const QString COATING_TYPE                      = "Type";
    const QString COATING_TYPE_EMISSIVE             = "Emissive";
    const QString COATING_TYPE_UNIFORMSTD           = "Uniform Std";
    const QString COATING_TYPE_TEXTUREDSTD          = "Textured Std";
    const QString COATING_ROUGHNESS                 = "Roughness";
    const QString COATING_PAINT_COLOR               = "Paint Color";
    const QString COATING_PAINT_REFRACTIVE_INDEX    = "Paint Refractive Index";
    const QString COATING_DEFAULT_ROUGHNESS         = "Default Roughness";
    const QString COATING_DEFAULT_PAINT_COLOR       = "Default Paint Color";
    const QString COATING_ROUGHNESS_TEX_NAME        = "Roughness Tex Name";
    const QString COATING_PAINT_COLOR_TEX_NAME      = "Paint Color Tex Name";
    const QString COATING_TEXTURE_FILTER            = "Texture Filter";
    const QString COATING_TEXTURE_WRAPPER           = "Texture Wrapper";
    const QString COATING_EMITTED_RADIANCE          = "Emitted Radiance";
    const QString COATING_AREA_SAMPLER              = "Area Sampler";
}
