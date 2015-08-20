#include "SceneWriter.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <QVariant>
#include <QVector3D>

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


namespace prop3
{
    SceneWriter::SceneWriter()
    {

    }

    SceneWriter::~SceneWriter()
    {

    }

    string SceneWriter::write(Scene& scene, bool prettyPrint)
    {
        // Scan scene
        _surfaceTreeMode = false;
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
        _propIdMap.clear();


        return std::string(doc.toJson(
            prettyPrint ? QJsonDocument::Indented :
                          QJsonDocument::Compact));
    }

    QJsonValue SceneWriter::toJson(const glm::dvec3& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        return array;
    }

    QJsonValue SceneWriter::toJson(const glm::dvec4& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
        array.append(v.a);
        return array;
    }

    QJsonValue SceneWriter::toJson(const glm::dmat4& m)
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

    bool SceneWriter::insertProp(Prop& node)
    {
        return _propIdMap.insert(make_pair(&node, _propIdMap.size())).second;
    }

    bool SceneWriter::insertSurface(ImplicitSurface& node)
    {
        return _surfaceIdMap.insert(make_pair(&node, _surfaceIdMap.size())).second;
    }

    bool SceneWriter::insertMaterial(Material& node)
    {
        return _materialIdMap.insert(make_pair(&node, _materialIdMap.size())).second;
    }

    bool SceneWriter::insertCoating(Coating& node)
    {
        return _coatingIdMap.insert(make_pair(&node, _coatingIdMap.size())).second;
    }


    // Props
    void SceneWriter::visit(Prop& node)
    {
        if(!_surfaceTreeMode)
        {
            if(insertProp(node))
            {
                QJsonObject obj;
                obj[PROP_TYPE] = PROP_TYPE_PROP;

                obj[PROP_MATERIAL] = _materialIdMap[node.material().get()];

                _surfaceTreeMode = true;
                if(node.surface().get() != nullptr)
                {
                    node.surface()->accept(*this);
                    obj[PROP_SURFACE] = _subTree;
                }

                if(node.boundingSurface().get() != nullptr)
                {
                    node.boundingSurface()->accept(*this);
                    obj[PROP_BOUNDING_SURFACE] = _subTree;
                }
                _surfaceTreeMode = false;

                _propsArray.append(obj);
            }
        }
    }


    // Implicit Surfaces
    void SceneWriter::visit(SurfaceGhost& node)
    {
        // Only appear in prop's surface tree
        if(_surfaceTreeMode)
        {
            auto children = node.children();
            assert(children.size() == 1);
            children[0]->accept(*this);

            QJsonObject logOpt;
            logOpt[SURFACE_OPERATOR_GHOST] = _subTree;
            _subTree = logOpt;
        }
    }

    void SceneWriter::visit(SurfaceInverse& node)
    {
        // Only appear in prop's surface tree
        if(_surfaceTreeMode)
        {
            auto children = node.children();
            assert(children.size() == 1);
            children[0]->accept(*this);

            QJsonObject logOpt;
            logOpt[SURFACE_OPERATOR_INVERSE] = _subTree;
            _subTree = logOpt;
        }
    }

    void SceneWriter::visit(SurfaceOr& node)
    {
        // Only appear in prop's surface tree
        if(_surfaceTreeMode)
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
    }

    void SceneWriter::visit(SurfaceAnd& node)
    {
        // Only appear in prop's surface tree
        if(_surfaceTreeMode)
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
    }

    void SceneWriter::visit(Plane& node)
    {
        if(!_surfaceTreeMode)
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
        else
        {
            _subTree = QJsonValue(_surfaceIdMap[&node]);
        }
    }

    void SceneWriter::visit(PlaneTexture& node)
    {
        if(!_surfaceTreeMode)
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
        else
        {
            _subTree = QJsonValue(_surfaceIdMap[&node]);
        }
    }

    void SceneWriter::visit(Quadric& node)
    {
        if(!_surfaceTreeMode)
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
        else
        {
            _subTree = QJsonValue(_surfaceIdMap[&node]);
        }
    }

    void SceneWriter::visit(Sphere& node)
    {
        if(!_surfaceTreeMode)
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
        else
        {
            _subTree = QJsonValue(_surfaceIdMap[&node]);
        }
    }


    // Materials
    void SceneWriter::visit(Air& node)
    {
        if(!_surfaceTreeMode)
        {
            if(insertMaterial(node))
            {
                QJsonObject obj;
                obj[MATERIAL_TYPE]             = MATERIAL_TYPE_AIR;
                obj[MATERIAL_REFRACTIVE_INDEX] = node.refractiveIndex();
                _materialsArray.append(obj);
            }
        }
    }

    void SceneWriter::visit(Concrete& node)
    {
        if(!_surfaceTreeMode)
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
    }

    void SceneWriter::visit(Glass& node)
    {
        if(!_surfaceTreeMode)
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
    }

    void SceneWriter::visit(Metal& node)
    {
        if(!_surfaceTreeMode)
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
    }


    // Coatings
    void SceneWriter::visit(NoCoating& node)
    {
        if(!_surfaceTreeMode)
        {
            if(insertCoating(node))
            {
                QJsonObject obj;
                obj[COATING_TYPE] = COATING_TYPE_NOCOATING;
                _coatingsArray.append(obj);
            }
        }
    }

    void SceneWriter::visit(FlatPaint& node)
    {
        if(!_surfaceTreeMode)
        {
            if(insertCoating(node))
            {
                QJsonObject obj;
                obj[COATING_TYPE]  = COATING_TYPE_FLATPAINT;
                obj[COATING_COLOR] = toJson(node.color());
                _coatingsArray.append(obj);
            }
        }
    }

    void SceneWriter::visit(GlossyPaint& node)
    {
        if(!_surfaceTreeMode)
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
    }

    void SceneWriter::visit(TexturedFlatPaint& node)
    {
        if(!_surfaceTreeMode)
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
    }

    void SceneWriter::visit(TexturedGlossyPaint& node)
    {
        if(!_surfaceTreeMode)
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
    }
}
