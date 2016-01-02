#include "JsonWriter.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <QVariant>
#include <QVector3D>

#include <CellarWorkbench/Misc/Log.h>

#include "JsonTags.h"

#include "Team/AbstractTeam.h"

#include "Node/StageSet.h"

#include "Prop/Prop.h"

#include "Prop/Surface/Box.h"
#include "Prop/Surface/Plane.h"
#include "Prop/Surface/Quadric.h"
#include "Prop/Surface/Sphere.h"

#include "Prop/Material/UniformStdMaterial.h"

#include "Prop/Coating/EmissiveCoating.h"
#include "Prop/Coating/UniformStdCoating.h"
#include "Prop/Coating/TexturedStdCoating.h"

#include "Light/Backdrop/ProceduralSun.h"
#include "Light/LightBulb/CircularLight.h"
#include "Light/LightBulb/SphericalLight.h"


using namespace std;
using namespace cellar;


namespace prop3
{
    StageSetJsonWriter::StageSetJsonWriter()
    {

    }

    StageSetJsonWriter::~StageSetJsonWriter()
    {

    }

    string StageSetJsonWriter::serialize(StageSet& stageSet, bool prettyPrint)
    {
        // Build Hardware dictionnary
        HardwareBuilder hardwareBuilder;
        stageSet.makeTraveling(hardwareBuilder);

        // Build Stage set tree
        StageSetBuilder stageSetBuilder(
                    hardwareBuilder.lightIdMap,
                    hardwareBuilder.materialIdMap,
                    hardwareBuilder.coatingIdMap,
                    hardwareBuilder.surfaceIdMap);
        stageSetBuilder.visit(stageSet);

        // Write document
        QJsonObject docObj;
        docObj[DOCUMENT_LIGHTS_ARRAY]   = hardwareBuilder.lightsArray;
        docObj[DOCUMENT_MATERIAL_ARRAY] = hardwareBuilder.materialsArray;
        docObj[DOCUMENT_COATING_ARRAY]  = hardwareBuilder.coatingsArray;
        docObj[DOCUMENT_SURFACE_ARRAY]  = hardwareBuilder.surfacesArray;
        docObj[DOCUMENT_STAGE_SET]      = stageSetBuilder.subTree;

        QJsonDocument doc;
        doc.setObject(docObj);
        return std::string(doc.toJson(
            prettyPrint ? QJsonDocument::Indented :
                          QJsonDocument::Compact));
    }

    QJsonValue StageSetJsonWriter::toJson(const glm::dvec3& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        return array;
    }

    QJsonValue StageSetJsonWriter::toJson(const glm::dvec4& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        array.append(v.a);
        return array;
    }

    QJsonValue StageSetJsonWriter::toJson(const glm::dmat4& m)
    {
        QJsonArray array;
        array.append(m[0][0]);
        array.append(m[0][1]);
        array.append(m[0][2]);
        array.append(m[0][3]);

        array.append(m[1][0]);
        array.append(m[1][1]);
        array.append(m[1][2]);
        array.append(m[1][3]);

        array.append(m[2][0]);
        array.append(m[2][1]);
        array.append(m[2][2]);
        array.append(m[2][3]);

        array.append(m[3][0]);
        array.append(m[3][1]);
        array.append(m[3][2]);
        array.append(m[3][3]);
        return array;
    }

    QJsonValue StageSetJsonWriter::toJson(const cellar::ESamplerFilter& filter)
    {
        switch(filter)
        {
        case cellar::ESamplerFilter::NEAREST :
            return "NEAREST";
        case cellar::ESamplerFilter::LINEAR :
            return "LINEAR";
        default:
            return "Unknow";
        }
    }

    QJsonValue StageSetJsonWriter::toJson(const cellar::ESamplerWrapper& wrapper)
    {
        switch(wrapper)
        {
        case cellar::ESamplerWrapper::CLAMP :
            return "CLAMP";
        case cellar::ESamplerWrapper::REPEAT :
            return "REPEAT";
        default:
            return "Unknow";
        }
    }

    bool StageSetJsonWriter::HardwareBuilder::insertLight(LightBulb& node)
    {
        return lightIdMap.insert(make_pair(&node, (int)lightIdMap.size())).second;
    }

    bool StageSetJsonWriter::HardwareBuilder::insertMaterial(Material& node)
    {
        return materialIdMap.insert(make_pair(&node, (int)materialIdMap.size())).second;
    }

    bool StageSetJsonWriter::HardwareBuilder::insertCoating(Coating& node)
    {
        return coatingIdMap.insert(make_pair(&node, (int)coatingIdMap.size())).second;
    }

    bool StageSetJsonWriter::HardwareBuilder::insertSurface(Surface& node)
    {
        return surfaceIdMap.insert(make_pair(&node, (int)surfaceIdMap.size())).second;
    }

    void StageSetJsonWriter::HardwareBuilder::setPhysicalProperties(PhysicalSurface& node, QJsonObject& obj)
    {
        obj[SURFACE_COATING]        = coatingIdMap[node.coating().get()];
        obj[SURFACE_INNER_MATERIAL] = materialIdMap[node.innerMaterial().get()];
        obj[SURFACE_OUTER_MATERIAL] = materialIdMap[node.outerMaterial().get()];
    }


    // Lights
    void StageSetJsonWriter::HardwareBuilder::visit(CircularLight& node)
    {
        if(insertLight(node))
        {
            QJsonObject obj;
            obj[LIGHT_TYPE]                 = LIGHT_TYPE_CIRCULAR;
            obj[LIGHT_IS_ON]                = node.isOn();
            obj[LIGHT_RADIANT_FLUX]         = toJson(node.radiantFlux());
            obj[LIGHT_TRANSFORM]            = toJson(node.transform());
            obj[LIGHT_CENTER]               = toJson(node.center());
            obj[LIGHT_NORMAL]               = toJson(node.normal());
            obj[LIGHT_RADIUS]               = node.radius();
            lightsArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(SphericalLight& node)
    {
        if(insertLight(node))
        {
            QJsonObject obj;
            obj[LIGHT_TYPE]                 = LIGHT_TYPE_SPHERICAL;
            obj[LIGHT_IS_ON]                = node.isOn();
            obj[LIGHT_RADIANT_FLUX]         = toJson(node.radiantFlux());
            obj[LIGHT_TRANSFORM]            = toJson(node.transform());
            obj[LIGHT_CENTER]               = toJson(node.center());
            obj[LIGHT_RADIUS]               = node.radius();
            lightsArray.append(obj);
        }
    }


    // Materials
    void StageSetJsonWriter::HardwareBuilder::visit(UniformStdMaterial& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]              = MATERIAL_TYPE_UNIFORMSTD;
            obj[MATERIAL_OPACITY]           = node.opacity();
            obj[MATERIAL_CONDUCTIVITY]      = node.conductivity();
            obj[MATERIAL_REFRACTIVE_INDEX]  = node.refractiveIndex();
            obj[MATERIAL_SCATTERING]        = node.scattering();
            obj[MATERIAL_COLOR]             = toJson(node.color());
            materialsArray.append(obj);
        }
    }


    // Coatings
    void StageSetJsonWriter::HardwareBuilder::visit(EmissiveCoating& node)
    {
        // Never add
    }

    void StageSetJsonWriter::HardwareBuilder::visit(UniformStdCoating& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]                   = COATING_TYPE_UNIFORMSTD;
            obj[COATING_ROUGHNESS]              = node.roughness();
            obj[COATING_PAINT_COLOR]            = toJson(node.paintColor());
            obj[COATING_PAINT_REFRACTIVE_INDEX] = node.paintRefractiveIndex();
            coatingsArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(TexturedStdCoating& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]                     = COATING_TYPE_TEXTUREDSTD;
            obj[COATING_DEFAULT_ROUGHNESS]        = node.defaultRoughness();
            obj[COATING_DEFAULT_PAINT_COLOR]      = toJson(node.defaultPaintColor());
            obj[COATING_ROUGHNESS_TEX_NAME]       = QString(node.roughnessTexName().c_str());
            obj[COATING_PAINT_COLOR_TEX_NAME]     = QString(node.paintColorTexName().c_str());
            obj[COATING_TEXTURE_FILTER]           = toJson(node.texFilter());
            obj[COATING_TEXTURE_WRAPPER]          = toJson(node.texWrapper());
            obj[COATING_PAINT_REFRACTIVE_INDEX]   = node.paintRefractiveIndex();
            coatingsArray.append(obj);
        }
    }


    // Surfaces
    void StageSetJsonWriter::HardwareBuilder::visit(Box& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]           = SURFACE_TYPE_BOX;
            obj[SURFACE_MIN_CORNER]     = toJson(node.minCorner());
            obj[SURFACE_MAX_CORNER]     = toJson(node.maxCorner());
            surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(BoxTexture& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]               = SURFACE_TYPE_BOX_TEXTURE;
            obj[SURFACE_MIN_CORNER]         = toJson(node.minCorner());
            obj[SURFACE_MAX_CORNER]         = toJson(node.maxCorner());
            obj[SURFACE_TEX_ORIGIN]         = toJson(node.texOrigin());
            obj[SURFACE_TEX_U_DIR]          = toJson(node.texU());
            obj[SURFACE_TEX_V_DIR]          = toJson(node.texV());
            obj[SURFACE_TEX_MAIN_SIDE_ONLY] = node.texMainSideOnly();
            surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(Plane& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]           = SURFACE_TYPE_PLANE;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(PlaneTexture& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]           = SURFACE_TYPE_PLANETEXTURE;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            obj[SURFACE_TEX_ORIGIN]     = toJson(node.texOrigin());
            obj[SURFACE_TEX_U_DIR]      = toJson(node.texU());
            obj[SURFACE_TEX_V_DIR]      = toJson(node.texV());
            surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(Quadric& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]           = SURFACE_TYPE_QUADRIC;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::HardwareBuilder::visit(Sphere& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            setPhysicalProperties(node, obj);
            obj[SURFACE_TYPE]    = SURFACE_TYPE_SPHERE;
            obj[SURFACE_RADIUS]  = node.radius();
            obj[SURFACE_CENTER]  = toJson(node.center());
            surfacesArray.append(obj);
        }
    }


    //////////////////////////
    // Surface Tree Builder //
    //////////////////////////
    StageSetJsonWriter::StageSetBuilder::StageSetBuilder(
            std::map<LightBulb*,int>& lightIdMap,
            std::map<Material*, int>& materialIdMap,
            std::map<Coating*,  int>& coatingIdMap,
            std::map<Surface*,  int>& surfaceIdMap) :
        _lightIdMap(lightIdMap),
        _materialIdMap(materialIdMap),
        _coatingIdMap(coatingIdMap),
        _surfaceIdMap(surfaceIdMap)
    {

    }

    // Stage set
    void StageSetJsonWriter::StageSetBuilder::visit(StageSet& node)
    {
        visit(static_cast<StageZone&>(node));
        QJsonObject obj = subTree.toObject();

        if(node.ambientMaterial().get() != nullptr)
        {
            node.ambientMaterial()->accept(*this);
            obj[STAGESET_AMBIENT_MATERIAL] = subTree;
        }

        if(node.backdrop().get() != nullptr)
        {
            node.backdrop()->accept(*this);
            obj[STAGESET_BACKDROP] = subTree;
        }

        subTree = obj;
    }

    // Zones
    void StageSetJsonWriter::StageSetBuilder::visit(StageZone& node)
    {
        QJsonObject obj;
        setHandleNodeProperties(node, obj);

        if(node.bounds().get() != nullptr)
        {
            obj[ZONE_BOUNDS] = _surfaceIdMap[node.bounds().get()];
        }

        if(!node.props().empty())
        {
            QJsonArray propArray;
            for(const auto& prop : node.props())
            {
                if(prop.get() != nullptr)
                {
                    prop->accept( *this );
                    propArray.append(subTree);
                }
            }
            obj[ZONE_PROPS] = propArray;
        }

        if(!node.lights().empty())
        {
            QJsonArray lightArray;
            for(const auto& light : node.lights())
            {
                lightArray.append(_lightIdMap[light.get()]);
            }
            obj[ZONE_LIGHTS] = lightArray;
        }

        if(!node.subzones().empty())
        {
            QJsonArray subzoneArray;
            for(const auto& subzone : node.subzones())
            {
                if(subzone.get() != nullptr)
                {
                    subzone->accept( *this );
                    subzoneArray.append(subTree);
                }
            }
            obj[ZONE_SUBZONES] = subzoneArray;
        }

        subTree = obj;
    }

    // Props
    void StageSetJsonWriter::StageSetBuilder::visit(Prop& node)
    {
        QJsonObject obj;
        setHandleNodeProperties(node, obj);

        QJsonArray surfArray;
        for(const auto& surf : node.surfaces())
        {
            if(surf.get() != nullptr)
            {
                surf->accept( *this );
                surfArray.append(subTree);
            }
        }
        obj[PROP_SURFACES] = surfArray;

        subTree = obj;
    }

    // Backdrop
    void StageSetJsonWriter::StageSetBuilder::visit(ProceduralSun& node)
    {
        QJsonObject obj;
        obj[BACKDROP_TYPE]                 = BACKDROP_TYPE_PROCEDURALSUN;
        obj[BACKDROP_SUN_COLOR]            = toJson(node.sunColor());
        obj[BACKDROP_SKY_COLOR]            = toJson(node.skyColor());
        obj[BACKDROP_SKYLINE_COLOR]        = toJson(node.skylineColor());
        obj[BACKDROP_GROUND_COLOR]         = toJson(node.groundColor());
        obj[BACKDROP_GROUND_HEIGHT]        = node.groundHeight();
        obj[BACKDROP_SUN_DIR]              = toJson(node.sunDirection());

        subTree = obj;
    }

    // Surfaces
    void StageSetJsonWriter::StageSetBuilder::visit(SurfaceShell& node)
    {
        auto children = node.children();
        assert(children.size() >= 1);
        children[0]->accept(*this);

        QJsonObject obj;
        obj[SURFACE_OPERATOR_SHELL] = subTree;
        obj[SURFACE_TRANSFORM] = toJson(node.transform());
        if(node.coating().get() != nullptr)
            obj[SURFACE_COATING] = _coatingIdMap[node.coating().get()];
        if(node.innerMaterial().get() != nullptr)
            obj[SURFACE_INNER_MATERIAL] = _materialIdMap[node.innerMaterial().get()];
        if(node.outerMaterial().get() != nullptr)
            obj[SURFACE_OUTER_MATERIAL] = _materialIdMap[node.outerMaterial().get()];

        subTree = obj;
    }

    void StageSetJsonWriter::StageSetBuilder::visit(SurfaceGhost& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject obj;
        obj[SURFACE_OPERATOR_GHOST] = subTree;

        subTree = obj;
    }

    void StageSetJsonWriter::StageSetBuilder::visit(SurfaceInverse& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject obj;
        obj[SURFACE_OPERATOR_INVERSE] = subTree;

        subTree = obj;
    }

    void StageSetJsonWriter::StageSetBuilder::visit(SurfaceOr& node)
    {
        QJsonArray childArray;
        for(auto surf : node.children())
        {
            surf->accept(*this);
            childArray.append(subTree);
        }

        QJsonObject obj;
        obj[SURFACE_OPERATOR_OR] = childArray;

        subTree = obj;
    }

    void StageSetJsonWriter::StageSetBuilder::visit(SurfaceAnd& node)
    {
        QJsonArray childArray;
        for(auto surf : node.children())
        {
            surf->accept(*this);
            childArray.append(subTree);
        }

        QJsonObject obj;
        obj[SURFACE_OPERATOR_AND] = childArray;

        subTree = obj;
    }

    void StageSetJsonWriter::StageSetBuilder::visit(Box& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::visit(BoxTexture& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::visit(Plane& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::visit(PlaneTexture& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::visit(Quadric& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::visit(Sphere& node)
    {
        subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::StageSetBuilder::setHandleNodeProperties(HandleNode& node, QJsonObject& obj)
    {
        if(!node.isVisible())
        obj[HANDLE_IS_VISIBLE] = node.isVisible();

        if(node.name() != "")
            obj[HANDLE_NAME] = node.name().c_str();
    }
}
