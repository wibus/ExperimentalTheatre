#include "SceneJsonWriter.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <QVariant>
#include <QVector3D>

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


namespace prop3
{
    SceneJsonWriter::SceneJsonWriter()
    {

    }

    SceneJsonWriter::~SceneJsonWriter()
    {

    }

    string SceneJsonWriter::write(Scene& scene, bool prettyPrint)
    {
        // Scan scene
        scene.makeTraveling(*this);

        // Write file
        QJsonObject sceneObj;
        sceneObj[SCENE_MATERIAL_ARRAY] = _materialsArray;
        sceneObj[SCENE_COATING_ARRAY]  = _coatingsArray;
        sceneObj[SCENE_SURFACE_ARRAY]  = _surfacesArray;
        sceneObj[SCENE_PROP_ARRAY]     = _propsArray;

        QJsonDocument doc;
        doc.setObject(sceneObj);


        //Clean-up structures
        _surfaceIdMap.clear();
        _materialIdMap.clear();
        _coatingIdMap.clear();


        return std::string(doc.toJson(
            prettyPrint ? QJsonDocument::Indented :
                          QJsonDocument::Compact));
    }

    QJsonValue SceneJsonWriter::toJson(const glm::dvec3& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        return array;
    }

    QJsonValue SceneJsonWriter::toJson(const glm::dvec4& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        array.append(v.a);
        return array;
    }

    QJsonValue SceneJsonWriter::toJson(const glm::dmat4& m)
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

    bool SceneJsonWriter::insertSurface(ImplicitSurface& node)
    {
        return _surfaceIdMap.insert(make_pair(&node, _surfaceIdMap.size())).second;
    }

    bool SceneJsonWriter::insertMaterial(Material& node)
    {
        return _materialIdMap.insert(make_pair(&node, _materialIdMap.size())).second;
    }

    bool SceneJsonWriter::insertCoating(Coating& node)
    {
        return _coatingIdMap.insert(make_pair(&node, _coatingIdMap.size())).second;
    }


    // Props
    void SceneJsonWriter::visit(Prop& node)
    {
        QJsonObject obj;
        obj[PROP_TYPE] = PROP_TYPE_PROP;

        obj[PROP_MATERIAL] = _materialIdMap[node.material().get()];

        if(node.surface().get() != nullptr)
        {
            SurfaceTreeBuilder builder(_surfaceIdMap);
            node.surface()->accept(builder);
            obj[PROP_SURFACE] = builder.surfaceTree();
        }

        if(node.boundingSurface().get() != nullptr)
        {
            SurfaceTreeBuilder builder(_surfaceIdMap);
            node.boundingSurface()->accept(builder);
            obj[PROP_BOUNDING_SURFACE] = builder.surfaceTree();
        }

        _propsArray.append(obj);
    }


    // Implicit Surfaces
    void SceneJsonWriter::visit(Plane& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            obj[SURFACE_TYPE]           = SURFACE_TYPE_PLANE;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            obj[SURFACE_COATING]        = _coatingIdMap[node.coating().get()];
            _surfacesArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(PlaneTexture& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            obj[SURFACE_TYPE]           = SURFACE_TYPE_PLANETEXTURE;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            obj[SURFACE_COATING]        = _coatingIdMap[node.coating().get()];
            obj[SURFACE_TEX_ORIGIN]     = toJson(node.texOrigin());
            obj[SURFACE_TEX_U_DIR]      = toJson(node.texU());
            obj[SURFACE_TEX_V_DIR]      = toJson(node.texV());
            _surfacesArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(Quadric& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            obj[SURFACE_TYPE]           = SURFACE_TYPE_QUADRIC;
            obj[SURFACE_REPRESENTATION] = toJson(node.representation());
            obj[SURFACE_COATING]        = _coatingIdMap[node.coating().get()];
            _surfacesArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(Sphere& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            obj[SURFACE_TYPE]    = SURFACE_TYPE_SPHERE;
            obj[SURFACE_RADIUS]  = node.radius();
            obj[SURFACE_CENTER]  = toJson(node.center());
            obj[SURFACE_COATING] = _coatingIdMap[node.coating().get()];
            _surfacesArray.append(obj);
        }
    }


    // Materials
    void SceneJsonWriter::visit(Air& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]             = MATERIAL_TYPE_AIR;
            obj[MATERIAL_REFRACTIVE_INDEX] = node.refractiveIndex();
            _materialsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(Concrete& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]             = MATERIAL_TYPE_CONCRETE;
            obj[MATERIAL_REFRACTIVE_INDEX] = node.refractiveIndex();
            obj[MATERIAL_COLOR]            = toJson(node.color());
            _materialsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(Glass& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]              = MATERIAL_TYPE_GLASS;
            obj[MATERIAL_REFRACTIVE_INDEX]  = node.refractiveIndex();
            obj[MATERIAL_COLOR]             = toJson(node.color());
            obj[MATERIAL_DYE_CONCENTRATION] = node.dyeConcentration();
            _materialsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(Metal& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]             = MATERIAL_TYPE_METAL;
            obj[MATERIAL_REFRACTIVE_INDEX] = node.refractiveIndex();
            obj[MATERIAL_COLOR]            = toJson(node.color());
            obj[MATERIAL_GLOSSINESS]       = node.glossiness();
            _materialsArray.append(obj);
        }
    }


    // Coatings
    void SceneJsonWriter::visit(NoCoating& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE] = COATING_TYPE_NOCOATING;
            _coatingsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(FlatPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]  = COATING_TYPE_FLATPAINT;
            obj[COATING_COLOR] = toJson(node.color());
            _coatingsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(GlossyPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]                     = COATING_TYPE_GLOSSYPAINT;
            obj[COATING_COLOR]                    = toJson(node.color());
            obj[COATING_GLOSSINESS]               = node.glossiness();
            obj[COATING_VARNISH_REFRACTIVE_INDEX] = node.varnishRefractiveIndex();
            _coatingsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(TexturedFlatPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]          = COATING_TYPE_TEXTUREDFLATPAINT;
            obj[COATING_DEFAULT_COLOR] = toJson(node.defaultColor());
            obj[COATING_TEXTURE_NAME]  = QString(node.texName().c_str());
            _coatingsArray.append(obj);
        }
    }

    void SceneJsonWriter::visit(TexturedGlossyPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]                     = COATING_TYPE_TEXTUREDGLOSSYPAINT;
            obj[COATING_DEFAULT_COLOR]            = toJson(node.defaultColor());
            obj[COATING_DEFAULT_GLOSS]            = node.defaultGlossiness();
            obj[COATING_TEXTURE_NAME]             = QString(node.texName().c_str());
            obj[COATING_GLOSS_MAP_NAME]           = QString(node.glossName().c_str());
            obj[COATING_VARNISH_REFRACTIVE_INDEX] = node.varnishRefractiveIndex();
            _coatingsArray.append(obj);
        }
    }

    SceneJsonWriter::SurfaceTreeBuilder::SurfaceTreeBuilder(
            std::map<ImplicitSurface*, int>& surfaceIdMap) :
        _surfaceIdMap(surfaceIdMap)
    {

    }

    const QJsonValue& SceneJsonWriter::SurfaceTreeBuilder::surfaceTree() const
    {
        return _subTree;
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(SurfaceGhost& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject logOpt;
        logOpt[SURFACE_OPERATOR_GHOST] = _subTree;
        _subTree = logOpt;
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(SurfaceInverse& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject logOpt;
        logOpt[SURFACE_OPERATOR_INVERSE] = _subTree;
        _subTree = logOpt;
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(SurfaceOr& node)
    {
        QJsonArray childArray;
        for(auto surf : node.children())
        {
            surf->accept(*this);
            childArray.append(_subTree);
        }

        QJsonObject localSubTree;
        localSubTree[SURFACE_OPERATOR_OR] = childArray;
        _subTree = localSubTree;
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(SurfaceAnd& node)
    {
        QJsonArray childArray;
        for(auto surf : node.children())
        {
            surf->accept(*this);
            childArray.append(_subTree);
        }

        QJsonObject localSubTree;
        localSubTree[SURFACE_OPERATOR_AND] = childArray;
        _subTree = localSubTree;
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(Plane& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(PlaneTexture& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(Quadric& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void SceneJsonWriter::SurfaceTreeBuilder::visit(Sphere& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }
}
