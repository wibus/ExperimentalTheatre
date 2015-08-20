#include "SceneJsonReader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <CellarWorkbench/Misc/Log.h>

#include "Scene.h"
#include "SceneJsonTags.h"

#include "Prop/Prop.h"

#include "Prop/ImplicitSurface/Plane.h"
#include "Prop/ImplicitSurface/Quadric.h"
#include "Prop/ImplicitSurface/Sphere.h"

#include "Prop/Coating/NoCoating.h"
#include "Prop/Coating/FlatPaint.h"
#include "Prop/Coating/GlossyPaint.h"
#include "Prop/Coating/TexturedFlatPaint.h"
#include "Prop/Coating/TexturedGlossyPaint.h"

#include "Prop/Material/Air.h"
#include "Prop/Material/Concrete.h"
#include "Prop/Material/Glass.h"
#include "Prop/Material/Metal.h"

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

    bool SceneJsonReader::read(Scene& scene, const std::string& stream)
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(stream.c_str());
        QJsonObject docObj = jsonDoc.object();

        // Deserialize coatings, materials and surfaces
        deserializeCoatings(docObj);
        deserializeMaterials(docObj);
        deserializeSurfaces(docObj);

        // Fill-in props
        deserializeProps(docObj, scene);

        //Clean-up structures
        _surfaces.clear();
        _materials.clear();
        _coatings.clear();

        return true;
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

    void SceneJsonReader::deserializeCoatings(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_COATING_ARRAY].toArray())
        {
            std::shared_ptr<Coating> coating;
            QJsonObject jsonObj = ref.toObject();
            QString type = jsonObj[COATING_TYPE].toString();

            if(type == COATING_TYPE_NOCOATING)
            {
                coating = make_shared<NoCoating>();
            }
            else if(type == COATING_TYPE_FLATPAINT)
            {
                coating = make_shared<FlatPaint>(
                    dvec3FromJson(jsonObj[COATING_COLOR]));
            }
            else if(type == COATING_TYPE_GLOSSYPAINT)
            {
                coating = make_shared<GlossyPaint>(
                    dvec3FromJson(jsonObj[COATING_COLOR]),
                    jsonObj[COATING_GLOSSINESS].toDouble(),
                    jsonObj[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else if(type == COATING_TYPE_TEXTUREDFLATPAINT)
            {
                coating = make_shared<TexturedFlatPaint>(
                    jsonObj[COATING_TEXTURE_NAME].toString().toStdString(),
                    dvec3FromJson(jsonObj[COATING_DEFAULT_COLOR]));
            }
            else if(type == COATING_TYPE_TEXTUREDGLOSSYPAINT)
            {
                coating = make_shared<TexturedGlossyPaint>(
                    jsonObj[COATING_TEXTURE_NAME].toString().toStdString(),
                    jsonObj[COATING_GLOSS_MAP_NAME].toString().toStdString(),
                    dvec3FromJson(jsonObj[COATING_DEFAULT_COLOR]),
                    jsonObj[COATING_DEFAULT_GLOSS].toDouble(),
                    jsonObj[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown coating type: " + type.toStdString(), "SceneReader"));
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
            QJsonObject jsonObj = ref.toObject();
            QString type = jsonObj[MATERIAL_TYPE].toString();

            if(type == MATERIAL_TYPE_AIR)
            {
                material = make_shared<Air>();
            }
            else if(type == MATERIAL_TYPE_CONCRETE)
            {
                material = make_shared<Concrete>(
                    dvec3FromJson(jsonObj[MATERIAL_COLOR]));
            }
            else if(type == MATERIAL_TYPE_GLASS)
            {
                material = make_shared<Glass>(
                    dvec3FromJson(jsonObj[MATERIAL_COLOR]),
                    jsonObj[MATERIAL_DYE_CONCENTRATION].toDouble());
            }
            else if(type == MATERIAL_TYPE_METAL)
            {
                material = make_shared<Metal>(
                    dvec3FromJson(jsonObj[MATERIAL_COLOR]),
                    jsonObj[MATERIAL_GLOSSINESS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown material type: " + type.toStdString(), "SceneReader"));
            }

            if(material.get() != nullptr)
            {
                material->setRefractiveIndex(
                    jsonObj[MATERIAL_REFRACTIVE_INDEX].toDouble());

                _materials.push_back(material);
            }
        }
    }

    void SceneJsonReader::deserializeSurfaces(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_SURFACE_ARRAY].toArray())
        {
            std::shared_ptr<ImplicitSurface> surface;
            QJsonObject jsonObj = ref.toObject();
            QString type = jsonObj[SURFACE_TYPE].toString();

            if(type == SURFACE_TYPE_PLANE)
            {
                surface = Plane::plane(
                    dvec4FromJson(jsonObj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_PLANETEXTURE)
            {
                surface = PlaneTexture::plane(
                    dvec4FromJson(jsonObj[SURFACE_REPRESENTATION]),
                    dvec3FromJson(jsonObj[SURFACE_TEX_U_DIR]),
                    dvec3FromJson(jsonObj[SURFACE_TEX_V_DIR]),
                    dvec3FromJson(jsonObj[SURFACE_TEX_ORIGIN]));
            }
            else if(type == SURFACE_TYPE_QUADRIC)
            {
                surface = Quadric::fromMatrix(
                    dmat4FromJson(jsonObj[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_SPHERE)
            {
                surface = Sphere::sphere(
                    dvec3FromJson(jsonObj[SURFACE_CENTER]),
                    jsonObj[SURFACE_RADIUS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface type: " + type.toStdString(), "SceneReader"));
            }

            if(surface.get() != nullptr)
            {
                surface->setCoating(_coatings[jsonObj[SURFACE_COATING].toInt()]);
                _surfaces.push_back(surface);
            }
        }
    }

    void SceneJsonReader::deserializeProps(const QJsonObject& sceneObj, Scene& scene)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_PROP_ARRAY].toArray())
        {
            std::shared_ptr<Prop> prop;
            QJsonObject jsonObj = ref.toObject();
            QString type = jsonObj[PROP_TYPE].toString();

            if(type == PROP_TYPE_PROP)
            {
                prop = scene.createProp();
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown prop type: " + type.toStdString(), "SceneReader"));
            }

            if(prop.get() != nullptr)
            {
                prop->setMaterial(_materials[jsonObj[PROP_MATERIAL].toInt()]);

                if(jsonObj.contains(PROP_SURFACE))
                {
                    prop->setSurface(subSurfTree(
                        jsonObj[PROP_SURFACE]));
                }

                if(jsonObj.contains(PROP_BOUNDING_SURFACE))
                {
                    prop->setBoundingSurface(subSurfTree(
                        jsonObj[PROP_BOUNDING_SURFACE]));
                }
            }
        }
    }

    std::shared_ptr<ImplicitSurface> SceneJsonReader::subSurfTree(
            const QJsonValue& surfaceTree)
    {
        if(surfaceTree.isDouble())
        {
            return _surfaces[surfaceTree.toInt()];
        }
        else
        {
            QJsonObject jsonObj = surfaceTree.toObject();
            assert(jsonObj.size() == 1);
            QString logOpt = jsonObj.begin().key();

            if(logOpt == SURFACE_OPERATOR_GHOST)
            {
                return ~subSurfTree(jsonObj[SURFACE_OPERATOR_GHOST]);
            }
            else if(logOpt == SURFACE_OPERATOR_INVERSE)
            {
                return !subSurfTree(jsonObj[SURFACE_OPERATOR_INVERSE]);
            }
            else if(logOpt == SURFACE_OPERATOR_OR)
            {
                vector<shared_ptr<ImplicitSurface>> operansSurf;
                for(QJsonValueRef ref : jsonObj[SURFACE_OPERATOR_OR].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                return SurfaceOr::apply(operansSurf);
            }
            else if(logOpt == SURFACE_OPERATOR_AND)
            {
                vector<shared_ptr<ImplicitSurface>> operansSurf;
                for(QJsonValueRef ref : jsonObj[SURFACE_OPERATOR_AND].toArray())
                    operansSurf.push_back(subSurfTree(ref));
                return SurfaceAnd::apply(operansSurf);
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface operator: " + logOpt.toStdString(), "SceneReader"));
            }
        }
    }
}
