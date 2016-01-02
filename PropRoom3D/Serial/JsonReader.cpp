#include "JsonReader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    StageSetJsonReader::StageSetJsonReader()
    {

    }

    StageSetJsonReader::~StageSetJsonReader()
    {

    }

    bool StageSetJsonReader::deserialize(
            AbstractTeam& team,
            const std::string& stream)
    {
        team.stageSet()->clear();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(stream.c_str());
        QJsonObject docObj = jsonDoc.object();

        // Deserialize hardware (coatings, materials and surfaces)
        deserializeLights(docObj);
        deserializeMaterials(docObj);
        deserializeCoatings(docObj);
        deserializeSurfaces(docObj);

        // Deserialize stage set tree
        deserializeStageSet(docObj[DOCUMENT_STAGE_SET], team);


        //Clean-up structures
        _lights.clear();
        _materials.clear();
        _coatings.clear();
        _surfaces.clear();

        return true;
    }

    glm::dvec3 StageSetJsonReader::dvec3FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec3(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble());
    }

    glm::dvec4 StageSetJsonReader::dvec4FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec4(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble(),
            array[3].toDouble());
    }

    glm::dmat4 StageSetJsonReader::dmat4FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dmat4(
                glm::dvec4(
                    array[0].toDouble(),
                    array[1].toDouble(),
                    array[2].toDouble(),
                    array[3].toDouble()),
                glm::dvec4(
                    array[4].toDouble(),
                    array[5].toDouble(),
                    array[6].toDouble(),
                    array[7].toDouble()),
                glm::dvec4(
                    array[8].toDouble(),
                    array[9].toDouble(),
                    array[10].toDouble(),
                    array[11].toDouble()),
                glm::dvec4(
                    array[12].toDouble(),
                    array[13].toDouble(),
                    array[14].toDouble(),
                    array[15].toDouble()));
    }

    cellar::ESamplerFilter StageSetJsonReader::filterFromJson(const QJsonValueRef& ref)
    {
        if(ref.toString() == "NEAREST")
            return cellar::ESamplerFilter::NEAREST;
        else
            return cellar::ESamplerFilter::LINEAR;
    }

    cellar::ESamplerWrapper StageSetJsonReader::wrapperFromJson(const QJsonValueRef& ref)
    {
        if(ref.toString() == "CLAMP")
            return cellar::ESamplerWrapper::CLAMP;
        else
            return cellar::ESamplerWrapper::REPEAT;
    }

    void StageSetJsonReader::deserializeLights(const QJsonObject& docObj)
    {
        for(QJsonValueRef ref : docObj[DOCUMENT_LIGHTS_ARRAY].toArray())
        {
            std::shared_ptr<LightBulb> node;
            QJsonObject obj = ref.toObject();
            QString type = obj[LIGHT_TYPE].toString();

            if(type == LIGHT_TYPE_CIRCULAR)
            {
                CircularLight* light = new CircularLight("",
                    dvec3FromJson(obj[LIGHT_CENTER]),
                    dvec3FromJson(obj[LIGHT_NORMAL]),
                    obj[LIGHT_RADIUS].toDouble());
                node.reset(light);
            }
            else if(type == LIGHT_TYPE_SPHERICAL)
            {
                SphericalLight* light = new SphericalLight("",
                    dvec3FromJson(obj[LIGHT_CENTER]),
                    obj[LIGHT_RADIUS].toDouble());
                node.reset(light);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown light type: " + type.toStdString(), "StageSetJsonReader"));
            }

            if(node.get() != nullptr)
            {
                setHandleNodeProperties(*node, obj);
                node->setIsOn(obj[LIGHT_IS_ON].toBool());
                node->setRadiantFlux(dvec3FromJson(obj[LIGHT_RADIANT_FLUX]));
                node->setTransform(dmat4FromJson(obj[LIGHT_TRANSFORM]));

                _lights.push_back(node);
            }
        }
    }

    void StageSetJsonReader::deserializeMaterials(const QJsonObject& docObj)
    {
        for(QJsonValueRef ref : docObj[DOCUMENT_MATERIAL_ARRAY].toArray())
        {
            std::shared_ptr<Material> node;
            QJsonObject obj = ref.toObject();
            QString type = obj[MATERIAL_TYPE].toString();

            if(type == MATERIAL_TYPE_UNIFORMSTD)
            {
                UniformStdMaterial* mat = new UniformStdMaterial();
                mat->setOpacity( obj[MATERIAL_OPACITY].toDouble() );
                mat->setConductivity( obj[MATERIAL_CONDUCTIVITY].toDouble() );
                mat->setRefractiveIndex( obj[MATERIAL_REFRACTIVE_INDEX].toDouble() );
                mat->setScattering( obj[MATERIAL_SCATTERING].toDouble() );
                mat->setColor( dvec3FromJson(obj[MATERIAL_COLOR]) );
                node.reset(mat);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown material type: " + type.toStdString(), "StageSetJsonReader"));
            }

            if(node.get() != nullptr)
            {
                _materials.push_back(node);
            }
        }
    }

    void StageSetJsonReader::deserializeCoatings(const QJsonObject& docObj)
    {
        for(QJsonValueRef ref : docObj[DOCUMENT_COATING_ARRAY].toArray())
        {
            std::shared_ptr<Coating> node;
            QJsonObject obj = ref.toObject();
            QString type = obj[COATING_TYPE].toString();

            if(type == COATING_TYPE_UNIFORMSTD)
            {
                UniformStdCoating* coat = new UniformStdCoating();
                coat->setRoughness( obj[COATING_ROUGHNESS].toDouble() );
                coat->setPaintColor( dvec4FromJson(obj[COATING_PAINT_COLOR]) );
                coat->setPaintRefractiveIndex( obj[COATING_PAINT_REFRACTIVE_INDEX].toDouble() );
                node.reset(coat);
            }
            else if(type == COATING_TYPE_TEXTUREDSTD)
            {
                TexturedStdCoating* coat = new TexturedStdCoating();
                coat->setDefaultRoughness( obj[COATING_DEFAULT_ROUGHNESS].toDouble() );
                coat->setDefaultPaintColor( dvec4FromJson(obj[COATING_DEFAULT_PAINT_COLOR]) );
                coat->setRoughnessTexName( obj[COATING_ROUGHNESS_TEX_NAME].toString().toStdString() );
                coat->setPaintColorTexName( obj[COATING_PAINT_COLOR_TEX_NAME].toString().toStdString() );
                coat->setPaintRefractiveIndex( obj[COATING_PAINT_REFRACTIVE_INDEX].toDouble() );
                coat->setTexFilter(  filterFromJson(obj[COATING_TEXTURE_FILTER]) );
                coat->setTexWrapper( wrapperFromJson(obj[COATING_TEXTURE_WRAPPER]) );
                node.reset(coat);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown coating type: " + type.toStdString(), "StageSetJsonReader"));
            }

            if(node.get() != nullptr)
            {
                _coatings.push_back(node);
            }
        }
    }

    void StageSetJsonReader::deserializeSurfaces(const QJsonObject& docObj)
    {
        for(QJsonValueRef ref : docObj[DOCUMENT_SURFACE_ARRAY].toArray())
        {
            std::shared_ptr<Surface> node;
            QJsonObject obj = ref.toObject();
            QString type = obj[SURFACE_TYPE].toString();

            if(type == SURFACE_TYPE_BOX)
            {
                node = Box::boxCorners(
                    dvec3FromJson(obj[SURFACE_MIN_CORNER]),
                    dvec3FromJson(obj[SURFACE_MAX_CORNER]));
            }
            else if(type == SURFACE_TYPE_BOX_TEXTURE)
            {
                node = BoxTexture::boxCorners(
                    dvec3FromJson(obj[SURFACE_MIN_CORNER]),
                    dvec3FromJson(obj[SURFACE_MAX_CORNER]),
                    dvec3FromJson(obj[SURFACE_TEX_ORIGIN]),
                    dvec3FromJson(obj[SURFACE_TEX_U_DIR]),
                    dvec3FromJson(obj[SURFACE_TEX_V_DIR]),
                    obj[SURFACE_TEX_MAIN_SIDE_ONLY].toBool());
            }
            else if(type == SURFACE_TYPE_PLANE)
            {
                node = Plane::plane(
                    dvec4FromJson(obj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_PLANETEXTURE)
            {
                node = PlaneTexture::plane(
                    dvec4FromJson(obj[SURFACE_REPRESENTATION]),
                    dvec3FromJson(obj[SURFACE_TEX_U_DIR]),
                    dvec3FromJson(obj[SURFACE_TEX_V_DIR]),
                    dvec3FromJson(obj[SURFACE_TEX_ORIGIN]));
            }
            else if(type == SURFACE_TYPE_QUADRIC)
            {
                node = Quadric::fromMatrix(
                    dmat4FromJson(obj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_SPHERE)
            {
                node = Sphere::sphere(
                    dvec3FromJson(obj[SURFACE_CENTER]),
                    obj[SURFACE_RADIUS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface type: " + type.toStdString(), "StageSetJsonReader"));
            }

            if(node.get() != nullptr)
            {
                node->setCoating(_coatings[obj[SURFACE_COATING].toInt()]);
                node->setInnerMaterial(_materials[obj[SURFACE_INNER_MATERIAL].toInt()]);
                node->setOuterMaterial(_materials[obj[SURFACE_OUTER_MATERIAL].toInt()]);
                _surfaces.push_back(node);
            }
        }
    }

    void StageSetJsonReader::deserializeStageSet(const QJsonValueRef& ref, AbstractTeam& team)
    {
        QJsonObject obj = ref.toObject();
        std::shared_ptr<StageSet> node = team.stageSet();
        setStageZoneProperties(*node, obj, team);

        if(obj.contains(STAGESET_AMBIENT_MATERIAL))
            node->setAmbientMaterial(_materials[obj[STAGESET_AMBIENT_MATERIAL].toInt()]);

        if(obj.contains(STAGESET_BACKDROP))
            node->setBackdrop(deserializeBackdrop(obj[STAGESET_BACKDROP], team));
    }

    std::shared_ptr<StageZone> StageSetJsonReader::deserializeZone(const QJsonValueRef& ref, AbstractTeam& team)
    {
        QJsonObject obj = ref.toObject();
        std::shared_ptr<StageZone> node(new StageZone(""));

        setStageZoneProperties(*node, obj, team);

        return node;
    }

    std::shared_ptr<Prop> StageSetJsonReader::deserializeProp(const QJsonValueRef& ref, AbstractTeam& team)
    {
        QJsonObject obj = ref.toObject();
        std::shared_ptr<Prop> node(new Prop(""));
        setHandleNodeProperties(*node, obj);

        if(obj.contains(PROP_SURFACES))
        {
            for(QJsonValueRef surfRef : obj[PROP_SURFACES].toArray())
            {
                auto surf = subSurfTree( surfRef );
                node->addSurface( surf );
            }
        }

        return node;
    }

    std::shared_ptr<Backdrop> StageSetJsonReader::deserializeBackdrop(const QJsonValueRef& ref, AbstractTeam& team)
    {
        QJsonObject obj = ref.toObject();
        std::shared_ptr<Backdrop> node;
        QString type = obj[BACKDROP_TYPE].toString();

        if(type == BACKDROP_TYPE_PROCEDURALSUN)
        {
            ProceduralSun* proceduralSun = new ProceduralSun();
            proceduralSun->setSunColor(dvec3FromJson(obj[BACKDROP_SUN_COLOR]));
            proceduralSun->setSkyColor(dvec3FromJson(obj[BACKDROP_SKY_COLOR]));
            proceduralSun->setSkylineColor(dvec3FromJson(obj[BACKDROP_SKYLINE_COLOR]));
            proceduralSun->setGroundColor(dvec3FromJson(obj[BACKDROP_GROUND_COLOR]));
            proceduralSun->setGroundHeight(obj[BACKDROP_GROUND_HEIGHT].toDouble());
            proceduralSun->setSunDirection(dvec3FromJson(obj[BACKDROP_SUN_DIR]));
            node.reset(proceduralSun);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Unknown backdrop type: " + type.toStdString(), "StageSetJsonReader"));
        }

        return node;
    }

    std::shared_ptr<Surface> StageSetJsonReader::subSurfTree(
            const QJsonValue& surfaceTree)
    {
        if(surfaceTree.isDouble())
        {
            return _surfaces[surfaceTree.toInt()];
        }
        else
        {
            std::shared_ptr<Surface> node;
            QJsonObject obj = surfaceTree.toObject();

            if(obj.contains(SURFACE_OPERATOR_SHELL))
            {
                node = Surface::shell(subSurfTree(obj[SURFACE_OPERATOR_SHELL]));
                Surface::transform(node, dmat4FromJson(obj[SURFACE_TRANSFORM]));

                if(obj.contains(SURFACE_COATING))
                    node->setCoating(_coatings[obj[SURFACE_COATING].toInt()]);
                if(obj.contains(SURFACE_INNER_MATERIAL))
                    node->setInnerMaterial(_materials[obj[SURFACE_INNER_MATERIAL].toInt()]);
                if(obj.contains(SURFACE_OUTER_MATERIAL))
                    node->setOuterMaterial(_materials[obj[SURFACE_OUTER_MATERIAL].toInt()]);
            }
            else if(obj.contains(SURFACE_OPERATOR_GHOST))
            {
                node = ~subSurfTree(obj[SURFACE_OPERATOR_GHOST]);
            }
            else if(obj.contains(SURFACE_OPERATOR_INVERSE))
            {
                node = !subSurfTree(obj[SURFACE_OPERATOR_INVERSE]);
            }
            else if(obj.contains(SURFACE_OPERATOR_OR))
            {
                vector<shared_ptr<Surface>> operansSurf;
                for(QJsonValueRef ref : obj[SURFACE_OPERATOR_OR].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                node = SurfaceOr::apply(operansSurf);
            }
            else if(obj.contains(SURFACE_OPERATOR_AND))
            {
                vector<shared_ptr<Surface>> operansSurf;
                for(QJsonValueRef ref : obj[SURFACE_OPERATOR_AND].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                node = SurfaceAnd::apply(operansSurf);
            }
            else
            {
                QString logOpt = obj.begin().key();
                getLog().postMessage(new Message('E', false,
                    "Unknown surface operator: " + logOpt.toStdString(), "StageSetJsonReader"));
            }

            if(node.get() != nullptr)
            {

            }

            return node;
        }
    }

    void StageSetJsonReader::setHandleNodeProperties(
            HandleNode& node,
            const QJsonObject& obj)
    {
        if(obj.contains(HANDLE_NAME))
            node.setName(obj[HANDLE_NAME].toString().toStdString());

        if(obj.contains((HANDLE_IS_VISIBLE)))
            node.setIsVisible(obj[HANDLE_IS_VISIBLE].toBool());
    }

    void StageSetJsonReader::setStageZoneProperties(
            StageZone& node,
            const QJsonObject& obj,
            AbstractTeam& team)
    {
        setHandleNodeProperties(node, obj);

        if(obj.contains(ZONE_BOUNDS))
            node.setBounds(subSurfTree( obj[ZONE_BOUNDS] ));
        else
            node.setBounds(StageZone::UNBOUNDED);

        if(obj.contains(ZONE_PROPS))
        {
            for(QJsonValueRef propRef : obj[ZONE_PROPS].toArray())
            {
                auto prop = deserializeProp(propRef, team);
                node.addProp(prop);
            }
        }

        if(obj.contains(ZONE_LIGHTS))
        {
            for(QJsonValueRef propRef : obj[ZONE_LIGHTS].toArray())
            {
                node.addLight(_lights[propRef.toInt()]);
            }
        }

        if(obj.contains(ZONE_SUBZONES))
        {
            for(QJsonValueRef zoneRef : obj[ZONE_SUBZONES].toArray())
            {
                auto subzone = deserializeZone(zoneRef, team);
                node.addSubzone(subzone);
            }
        }
    }
}
