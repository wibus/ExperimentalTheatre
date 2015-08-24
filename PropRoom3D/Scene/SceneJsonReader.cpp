#include "SceneJsonReader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <CellarWorkbench/Misc/Log.h>
#include <CellarWorkbench/Misc/StringUtils.h>

#include "Scene.h"
#include "SceneJsonTags.h"

#include <Team/AbstractTeam.h>

#include "Prop/Prop.h"

#include "Prop/Surface/Plane.h"
#include "Prop/Surface/Quadric.h"
#include "Prop/Surface/Sphere.h"

#include "Prop/Coating/NoCoating.h"
#include "Prop/Coating/FlatPaint.h"
#include "Prop/Coating/GlossyPaint.h"
#include "Prop/Coating/TexturedFlatPaint.h"
#include "Prop/Coating/TexturedGlossyPaint.h"

#include "Prop/Material/Air.h"
#include "Prop/Material/Concrete.h"
#include "Prop/Material/Glass.h"
#include "Prop/Material/Metal.h"

#include "Environment/Environment.h"

#include "Environment/Backdrop/ProceduralSun.h"

using namespace std;
using namespace cellar;


namespace prop3
{
    SceneJsonReader::SceneJsonReader()
    {

    }

    SceneJsonReader::~SceneJsonReader()
    {

    }

    bool SceneJsonReader::deserialize(
            AbstractTeam& team,
            const std::string& stream,
            bool clearScene)
    {
        if(clearScene)
        {
            team.clearProps();
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(stream.c_str());
        QJsonObject docObj = jsonDoc.object();

        // Deserialize coatings, materials, surfaces and backdrops
        deserializeCoatings(docObj);
        deserializeMaterials(docObj);
        deserializeSurfaces(docObj);
        deserializeBackdrops(docObj);

        // Fill-in props
        deserializeProps(docObj, team);

        // Fill-in environment
        deserializeEnvironment(docObj, team);


        //Clean-up structures
        _backdrops.clear();
        _surfaces.clear();
        _materials.clear();
        _coatings.clear();

        return true;
    }

    bool SceneJsonReader::loadFromFile(
            AbstractTeam& team,
            const std::string& fileName,
            bool clearScene)
    {
        bool ok = false;
        string stream = fileToString(fileName, &ok);

        if(ok)
        {
            deserialize(team, stream, clearScene);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Scene failed to load from '" + fileName + "'." +
                string(clearScene ? " Scene won't be cleared." : ""),
                "SceneJsonReader"));
        }
    }

    glm::dvec3 SceneJsonReader::dvec3FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec3(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble());
    }

    glm::dvec4 SceneJsonReader::dvec4FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec4(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble(),
            array[3].toDouble());
    }

    glm::dmat4 SceneJsonReader::dmat4FromJson(const QJsonValueRef& ref)
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

    void SceneJsonReader::deserializeBackdrops(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_BACKDROP_ARRAY].toArray())
        {
            std::shared_ptr<Backdrop> backdrop;
            QJsonObject obj = ref.toObject();
            QString type = obj[COATING_TYPE].toString();

            if(type == BACKDROP_TYPE_PROCEDURALSUN)
            {
                ProceduralSun* proceduralSun = new ProceduralSun();
                proceduralSun->setSunColor(dvec3FromJson(obj[BACKDROP_SUN_COLOR]));
                proceduralSun->setSkyColor(dvec3FromJson(obj[BACKDROP_SKY_COLOR]));
                proceduralSun->setSkylineColor(dvec3FromJson(obj[BACKDROP_SKYLINE_COLOR]));
                proceduralSun->setUpSkyDirection(dvec3FromJson(obj[BACKDROP_UP_SKY_DIR]));
                proceduralSun->setSunDirection(dvec3FromJson(obj[BACKDROP_SUN_DIR]));
                backdrop.reset(proceduralSun);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown backdrop type: " + type.toStdString(), "SceneJsonReader"));
            }

            if(backdrop.get() != nullptr)
            {
                backdrop->setIsDirectlyVisible(obj[BACKDROP_IS_DIRECTLY_VISIBLE].toBool());
                _backdrops.push_back(backdrop);
            }
        }
    }

    void SceneJsonReader::deserializeEnvironment(const QJsonObject& sceneObj, AbstractTeam& team)
    {
        std::shared_ptr<Environment> environment;
        QJsonObject obj = sceneObj[SCENE_ENVIRONMENT_OBJECT].toObject();
        QString type = obj[ENVIRONMENT_TYPE].toString();

        if(type == ENVIRONMENT_TYPE_ENVIRONMENT)
        {
            environment = team.scene()->environment();
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Unknown environment type: " + type.toStdString(), "SceneJsonReader"));
        }

        if(environment.get() != nullptr)
        {
            if(obj.contains(ENVIRONMENT_BACKDROP))
                environment->setBackdrop(_backdrops[obj[ENVIRONMENT_BACKDROP].toInt()]);
            environment->setAmbientMaterial(
                _materials[obj[ENVIRONMENT_AMBIENT_MATERIAL].toInt()]);
        }
    }

    void SceneJsonReader::deserializeCoatings(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_COATING_ARRAY].toArray())
        {
            std::shared_ptr<Coating> coating;
            QJsonObject obj = ref.toObject();
            QString type = obj[COATING_TYPE].toString();

            if(type == COATING_TYPE_NOCOATING)
            {
                coating = make_shared<NoCoating>();
            }
            else if(type == COATING_TYPE_FLATPAINT)
            {
                coating = make_shared<FlatPaint>(
                    dvec3FromJson(obj[COATING_COLOR]));
            }
            else if(type == COATING_TYPE_GLOSSYPAINT)
            {
                coating = make_shared<GlossyPaint>(
                    dvec3FromJson(obj[COATING_COLOR]),
                    obj[COATING_GLOSSINESS].toDouble(),
                    obj[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else if(type == COATING_TYPE_TEXTUREDFLATPAINT)
            {
                coating = make_shared<TexturedFlatPaint>(
                    obj[COATING_TEXTURE_NAME].toString().toStdString(),
                    dvec3FromJson(obj[COATING_DEFAULT_COLOR]));
            }
            else if(type == COATING_TYPE_TEXTUREDGLOSSYPAINT)
            {
                coating = make_shared<TexturedGlossyPaint>(
                    obj[COATING_TEXTURE_NAME].toString().toStdString(),
                    obj[COATING_GLOSS_MAP_NAME].toString().toStdString(),
                    dvec3FromJson(obj[COATING_DEFAULT_COLOR]),
                    obj[COATING_DEFAULT_GLOSS].toDouble(),
                    obj[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown coating type: " + type.toStdString(), "SceneJsonReader"));
            }

            if(coating.get() != nullptr)
            {
                _coatings.push_back(coating);
            }
        }
    }

    void SceneJsonReader::deserializeMaterials(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_MATERIAL_ARRAY].toArray())
        {
            std::shared_ptr<Material> material;
            QJsonObject obj = ref.toObject();
            QString type = obj[MATERIAL_TYPE].toString();

            if(type == MATERIAL_TYPE_AIR)
            {
                material = make_shared<Air>();
            }
            else if(type == MATERIAL_TYPE_CONCRETE)
            {
                material = make_shared<Concrete>(
                    dvec3FromJson(obj[MATERIAL_COLOR]));
            }
            else if(type == MATERIAL_TYPE_GLASS)
            {
                material = make_shared<Glass>(
                    dvec3FromJson(obj[MATERIAL_COLOR]),
                    obj[MATERIAL_DYE_CONCENTRATION].toDouble());
            }
            else if(type == MATERIAL_TYPE_METAL)
            {
                material = make_shared<Metal>(
                    dvec3FromJson(obj[MATERIAL_COLOR]),
                    obj[MATERIAL_GLOSSINESS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown material type: " + type.toStdString(), "SceneJsonReader"));
            }

            if(material.get() != nullptr)
            {
                material->setRefractiveIndex(
                    obj[MATERIAL_REFRACTIVE_INDEX].toDouble());

                _materials.push_back(material);
            }
        }
    }

    void SceneJsonReader::deserializeSurfaces(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_SURFACE_ARRAY].toArray())
        {
            std::shared_ptr<Surface> surface;
            QJsonObject obj = ref.toObject();
            QString type = obj[SURFACE_TYPE].toString();

            if(type == SURFACE_TYPE_PLANE)
            {
                surface = Plane::plane(
                    dvec4FromJson(obj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_PLANETEXTURE)
            {
                surface = PlaneTexture::plane(
                    dvec4FromJson(obj[SURFACE_REPRESENTATION]),
                    dvec3FromJson(obj[SURFACE_TEX_U_DIR]),
                    dvec3FromJson(obj[SURFACE_TEX_V_DIR]),
                    dvec3FromJson(obj[SURFACE_TEX_ORIGIN]));
            }
            else if(type == SURFACE_TYPE_QUADRIC)
            {
                surface = Quadric::fromMatrix(
                    dmat4FromJson(obj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_SPHERE)
            {
                surface = Sphere::sphere(
                    dvec3FromJson(obj[SURFACE_CENTER]),
                    obj[SURFACE_RADIUS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface type: " + type.toStdString(), "SceneJsonReader"));
            }

            if(surface.get() != nullptr)
            {
                surface->setCoating(_coatings[obj[SURFACE_COATING].toInt()]);
                _surfaces.push_back(surface);
            }
        }
    }

    void SceneJsonReader::deserializeProps(const QJsonObject& sceneObj, AbstractTeam& team)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_PROP_ARRAY].toArray())
        {
            std::shared_ptr<Prop> prop;
            QJsonObject obj = ref.toObject();
            QString type = obj[PROP_TYPE].toString();

            if(type == PROP_TYPE_PROP)
            {
                prop = team.createProp();
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown prop type: " + type.toStdString(), "SceneJsonReader"));
            }

            if(prop.get() != nullptr)
            {
                prop->setMaterial(_materials[obj[PROP_MATERIAL].toInt()]);

                if(obj.contains(PROP_SURFACE))
                {
                    prop->setSurface(subSurfTree(
                        obj[PROP_SURFACE]));
                }

                if(obj.contains(PROP_BOUNDING_SURFACE))
                {
                    prop->setBoundingSurface(subSurfTree(
                        obj[PROP_BOUNDING_SURFACE]));
                }
            }
        }
    }

    std::shared_ptr<Surface> SceneJsonReader::subSurfTree(
            const QJsonValue& surfaceTree)
    {
        if(surfaceTree.isDouble())
        {
            return _surfaces[surfaceTree.toInt()];
        }
        else
        {
            QJsonObject obj = surfaceTree.toObject();
            assert(obj.size() == 1);
            QString logOpt = obj.begin().key();

            if(logOpt == SURFACE_OPERATOR_GHOST)
            {
                return ~subSurfTree(obj[SURFACE_OPERATOR_GHOST]);
            }
            else if(logOpt == SURFACE_OPERATOR_INVERSE)
            {
                return !subSurfTree(obj[SURFACE_OPERATOR_INVERSE]);
            }
            else if(logOpt == SURFACE_OPERATOR_OR)
            {
                vector<shared_ptr<Surface>> operansSurf;
                for(QJsonValueRef ref : obj[SURFACE_OPERATOR_OR].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                return SurfaceOr::apply(operansSurf);
            }
            else if(logOpt == SURFACE_OPERATOR_AND)
            {
                vector<shared_ptr<Surface>> operansSurf;
                for(QJsonValueRef ref : obj[SURFACE_OPERATOR_AND].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                return SurfaceAnd::apply(operansSurf);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface operator: " + logOpt.toStdString(), "SceneJsonReader"));
            }
        }
    }
}
