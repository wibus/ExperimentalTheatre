#include "SceneReader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <CellarWorkbench/Misc/Log.h>

#include "Scene.h"
#include "SceneJson.h"

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
    SceneReader::SceneReader()
    {

    }

    SceneReader::~SceneReader()
    {

    }

    bool SceneReader::read(Scene& scene, const std::string& stream)
    {
        QJsonDocument doc = QJsonDocument::fromJson(stream.c_str());
        QJsonObject docObj = doc.object();

        // Deserialize objects
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

    glm::dvec3 SceneReader::dvec3FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec3(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble());
    }

    glm::dvec4 SceneReader::dvec4FromJson(const QJsonValueRef& ref)
    {
        QJsonArray array = ref.toArray();
        return glm::dvec4(
            array[0].toDouble(),
            array[1].toDouble(),
            array[2].toDouble(),
            array[3].toDouble());
    }

    glm::dmat4 SceneReader::dmat4FromJson(const QJsonValueRef& ref)
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

    void SceneReader::deserializeCoatings(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_COATING_ARRAY].toArray())
        {
            std::shared_ptr<Coating> obj;
            QJsonObject coating = ref.toObject();
            QString type = coating[COATING_TYPE].toString();

            if(type == COATING_TYPE_NOCOATING)
            {
                obj = make_shared<NoCoating>();
            }
            else if(type == COATING_TYPE_FLATPAINT)
            {
                obj = make_shared<FlatPaint>(
                    dvec3FromJson(coating[COATING_COLOR]));
            }
            else if(type == COATING_TYPE_GLOSSYPAINT)
            {
                obj = make_shared<GlossyPaint>(
                    dvec3FromJson(coating[COATING_COLOR]),
                    coating[COATING_GLOSSINESS].toDouble(),
                    coating[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else if(type == COATING_TYPE_TEXTUREDFLATPAINT)
            {
                obj = make_shared<TexturedFlatPaint>(
                    coating[COATING_TEXTURE_NAME].toString().toStdString(),
                    dvec3FromJson(coating[COATING_DEFAULT_COLOR]));
            }
            else if(type == COATING_TYPE_TEXTUREDGLOSSYPAINT)
            {
                obj = make_shared<TexturedGlossyPaint>(
                    coating[COATING_TEXTURE_NAME].toString().toStdString(),
                    coating[COATING_GLOSS_MAP_NAME].toString().toStdString(),
                    dvec3FromJson(coating[COATING_DEFAULT_COLOR]),
                    coating[COATING_DEFAULT_GLOSS].toDouble(),
                    coating[COATING_VARNISH_REFRACTIVE_INDEX].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown coating type: " + type.toStdString(), "SceneReader"));
            }

            if(obj.get() != nullptr)
            {
                _coatings.push_back(obj);
            }
        }
    }

    void SceneReader::deserializeMaterials(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_MATERIAL_ARRAY].toArray())
        {
            QJsonObject material = ref.toObject();
            QString type = material[MATERIAL_TYPE].toString();
            std::shared_ptr<Material> obj;

            if(type == MATERIAL_TYPE_AIR)
            {
                obj = make_shared<Air>(
                    material[MATERIAL_REFRACTIVE_INDEX].toDouble());
            }
            else if(type == MATERIAL_TYPE_CONCRETE)
            {
                obj = make_shared<Concrete>(
                    dvec3FromJson(material[MATERIAL_COLOR]));
            }
            else if(type == MATERIAL_TYPE_GLASS)
            {
                obj = make_shared<Glass>(
                    dvec3FromJson(material[MATERIAL_COLOR]),
                    material[MATERIAL_DYE_CONCENTRATION].toDouble(),
                    material[MATERIAL_REFRACTIVE_INDEX].toDouble());
            }
            else if(type == MATERIAL_TYPE_METAL)
            {
                obj = make_shared<Metal>(
                    dvec3FromJson(material[MATERIAL_COLOR]),
                    material[MATERIAL_GLOSSINESS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown material type: " + type.toStdString(), "SceneReader"));
            }

            if(obj.get() != nullptr)
            {
                _materials.push_back(obj);
            }
        }
    }

    void SceneReader::deserializeSurfaces(const QJsonObject& sceneObj)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_SURFACE_ARRAY].toArray())
        {
            QJsonObject surface = ref.toObject();
            QString type = surface[SURFACE_TYPE].toString();

            std::shared_ptr<ImplicitSurface> obj;

            if(type == SURFACE_TYPE_PLANE)
            {
                obj = Plane::plane(
                    dvec4FromJson(surface[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_PLANETEXTURE)
            {
                obj = PlaneTexture::plane(
                    dvec4FromJson(surface[SURFACE_REPRESENTATION]),
                    dvec3FromJson(surface[SURFACE_TEX_U_DIR]),
                    dvec3FromJson(surface[SURFACE_TEX_V_DIR]),
                    dvec3FromJson(surface[SURFACE_TEX_ORIGIN]));
            }
            else if(type == SURFACE_TYPE_QUADRIC)
            {
                obj = Quadric::fromMatrix(
                    dmat4FromJson(surface[SURFACE_REPRESENTATION]));
            }
            else if(type == SURFACE_TYPE_SPHERE)
            {
                obj = Sphere::sphere(
                    dvec3FromJson(surface[SURFACE_CENTER]),
                    surface[SURFACE_RADIUS].toDouble());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown surface type: " + type.toStdString(), "SceneReader"));
            }

            if(obj.get() != nullptr)
            {
                obj->setCoating(_coatings[surface[SURFACE_COATING].toInt()]);
                _surfaces.push_back(obj);
            }
        }
    }

    void SceneReader::deserializeProps(const QJsonObject& sceneObj, Scene& scene)
    {
        for(QJsonValueRef ref : sceneObj[SCENE_PROP_ARRAY].toArray())
        {
            std::shared_ptr<Prop> obj;
            QJsonObject prop = ref.toObject();
            QString type = prop[PROP_TYPE].toString();

            if(type == PROP_TYPE_PROP)
            {
                obj = scene.createProp();
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Unknown prop type: " + type.toStdString(), "SceneReader"));
            }

            if(obj.get() != nullptr)
            {
                obj->setMaterial(_materials[prop[PROP_MATERIAL].toInt()]);

                if(prop.contains(PROP_SURFACE))
                {
                    obj->setSurface(subSurfTree(
                        prop[PROP_SURFACE]));
                }

                if(prop.contains(PROP_BOUNDING_SURFACE))
                {
                    obj->setBoundingSurface(subSurfTree(
                        prop[PROP_BOUNDING_SURFACE]));
                }
            }
        }
    }

    std::shared_ptr<ImplicitSurface> SceneReader::subSurfTree(
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
