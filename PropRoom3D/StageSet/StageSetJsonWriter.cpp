#include "StageSetJsonWriter.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <QVariant>
#include <QVector3D>

#include <CellarWorkbench/Misc/Log.h>

#include "StageSet.h"
#include "StageSetJsonTags.h"

#include "Team/AbstractTeam.h"

#include "Prop/Prop.h"

#include "Prop/Surface/Box.h"
#include "Prop/Surface/Plane.h"
#include "Prop/Surface/Quadric.h"
#include "Prop/Surface/Sphere.h"

#include "Prop/Coating/NoCoating.h"
#include "Prop/Coating/FlatPaint.h"
#include "Prop/Coating/GlossyPaint.h"
#include "Prop/Coating/TexturedFlatPaint.h"
#include "Prop/Coating/TexturedGlossyPaint.h"

#include "Prop/Material/Air.h"
#include "Prop/Material/Fog.h"
#include "Prop/Material/Concrete.h"
#include "Prop/Material/Glass.h"
#include "Prop/Material/Metal.h"

#include "Lighting/Environment.h"
#include "Lighting/Backdrop/ProceduralSun.h"

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
        // Scan stageSet
        stageSet.makeTraveling(*this);

        // Write file
        QJsonObject stageSetObj;
        stageSetObj[STAGESET_ENVIRONMENT_OBJECT] = _environmentObject;
        stageSetObj[STAGESET_BACKDROP_ARRAY]     = _backdropsArray;
        stageSetObj[STAGESET_MATERIAL_ARRAY]     = _materialsArray;
        stageSetObj[STAGESET_COATING_ARRAY]      = _coatingsArray;
        stageSetObj[STAGESET_SURFACE_ARRAY]      = _surfacesArray;
        stageSetObj[STAGESET_PROP_ARRAY]         = _propsArray;

        QJsonDocument doc;
        doc.setObject(stageSetObj);


        //Clean-up structures
        _backdropIdMap.clear();
        _surfaceIdMap.clear();
        _materialIdMap.clear();
        _coatingIdMap.clear();

        _environmentObject = QJsonObject();
        _backdropsArray = QJsonArray();
        _propsArray = QJsonArray();
        _surfacesArray = QJsonArray();
        _materialsArray = QJsonArray();
        _coatingsArray = QJsonArray();


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

    bool StageSetJsonWriter::insertBackdrop(Backdrop& node)
    {
        return _backdropIdMap.insert(make_pair(&node, _backdropIdMap.size())).second;
    }

    bool StageSetJsonWriter::insertSurface(Surface& node)
    {
        return _surfaceIdMap.insert(make_pair(&node, _surfaceIdMap.size())).second;
    }

    bool StageSetJsonWriter::insertMaterial(Material& node)
    {
        return _materialIdMap.insert(make_pair(&node, _materialIdMap.size())).second;
    }

    bool StageSetJsonWriter::insertCoating(Coating& node)
    {
        return _coatingIdMap.insert(make_pair(&node, _coatingIdMap.size())).second;
    }


    // Props
    void StageSetJsonWriter::visit(Prop& node)
    {
        QJsonObject obj;
        obj[PROP_TYPE] = PROP_TYPE_PROP;

        obj[PROP_MATERIAL] = _materialIdMap[node.material().get()];

        if(node.surface().get() != nullptr)
        {
            SurfaceTreeBuilder builder(_surfaceIdMap, _coatingIdMap);
            node.surface()->accept(builder);
            obj[PROP_SURFACE] = builder.surfaceTree();
        }

        if(node.boundingSurface().get() != nullptr)
        {
            SurfaceTreeBuilder builder(_surfaceIdMap, _coatingIdMap);
            node.boundingSurface()->accept(builder);
            obj[PROP_BOUNDING_SURFACE] = builder.surfaceTree();
        }

        _propsArray.append(obj);
    }


    // Implicit Surfaces
    void StageSetJsonWriter::visit(Box& node)
    {
        if(insertSurface(node))
        {
            QJsonObject obj;
            obj[SURFACE_TYPE]           = SURFACE_TYPE_BOX;
            obj[SURFACE_MIN_CORNER]      = toJson(node.minCorner());
            obj[SURFACE_MAX_CORNER]      = toJson(node.maxCorner());
            obj[SURFACE_COATING]        = _coatingIdMap[node.coating().get()];
            _surfacesArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(Plane& node)
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

    void StageSetJsonWriter::visit(PlaneTexture& node)
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

    void StageSetJsonWriter::visit(Quadric& node)
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

    void StageSetJsonWriter::visit(Sphere& node)
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
    void StageSetJsonWriter::visit(Air& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]             = MATERIAL_TYPE_AIR;
            obj[MATERIAL_REFRACTIVE_INDEX] = node.refractiveIndex();
            _materialsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(Fog& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]              = MATERIAL_TYPE_FOG;
            obj[MATERIAL_REFRACTIVE_INDEX]  = node.refractiveIndex();
            obj[MATERIAL_COLOR]             = toJson(node.color());
            obj[MATERIAL_CONCENTRATION]     = node.concentration();
            obj[MATERIAL_RADIUS]            = node.radius();
            _materialsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(Concrete& node)
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

    void StageSetJsonWriter::visit(Glass& node)
    {
        if(insertMaterial(node))
        {
            QJsonObject obj;
            obj[MATERIAL_TYPE]              = MATERIAL_TYPE_GLASS;
            obj[MATERIAL_REFRACTIVE_INDEX]  = node.refractiveIndex();
            obj[MATERIAL_COLOR]             = toJson(node.color());
            obj[MATERIAL_CONCENTRATION]     = node.concentration();
            _materialsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(Metal& node)
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
    void StageSetJsonWriter::visit(NoCoating& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE] = COATING_TYPE_NOCOATING;
            _coatingsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(FlatPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]  = COATING_TYPE_FLATPAINT;
            obj[COATING_COLOR] = toJson(node.color());
            _coatingsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(GlossyPaint& node)
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

    void StageSetJsonWriter::visit(TexturedFlatPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]            = COATING_TYPE_TEXTUREDFLATPAINT;
            obj[COATING_DEFAULT_COLOR]   = toJson(node.defaultColor());
            obj[COATING_TEXTURE_NAME]    = QString(node.texName().c_str());
            obj[COATING_TEXTURE_FILTER]  = toJson(node.texFilter());
            obj[COATING_TEXTURE_WRAPPER] = toJson(node.texWrapper());
            _coatingsArray.append(obj);
        }
    }

    void StageSetJsonWriter::visit(TexturedGlossyPaint& node)
    {
        if(insertCoating(node))
        {
            QJsonObject obj;
            obj[COATING_TYPE]                     = COATING_TYPE_TEXTUREDGLOSSYPAINT;
            obj[COATING_DEFAULT_COLOR]            = toJson(node.defaultColor());
            obj[COATING_DEFAULT_GLOSS]            = node.defaultGlossiness();
            obj[COATING_TEXTURE_NAME]             = QString(node.texName().c_str());
            obj[COATING_TEXTURE_FILTER]           = toJson(node.texFilter());
            obj[COATING_TEXTURE_WRAPPER]          = toJson(node.texWrapper());
            obj[COATING_GLOSS_MAP_NAME]           = QString(node.glossName().c_str());
            obj[COATING_VARNISH_REFRACTIVE_INDEX] = node.varnishRefractiveIndex();
            _coatingsArray.append(obj);
        }
    }


    // Environments
    void StageSetJsonWriter::visit(Environment& node)
    {
        _environmentObject[ENVIRONMENT_TYPE]              = ENVIRONMENT_TYPE_ENVIRONMENT;
        _environmentObject[ENVIRONMENT_AMBIENT_MATERIAL]  = _materialIdMap[node.ambientMaterial().get()];

        if(node.backdrop().get() != nullptr)
        {
            _environmentObject[ENVIRONMENT_BACKDROP] = _backdropIdMap[node.backdrop().get()];
        }
    }


    // Backdrops
    void StageSetJsonWriter::visit(ProceduralSun& node)
    {
        if(insertBackdrop(node))
        {
            QJsonObject obj;
            obj[BACKDROP_TYPE]                 = BACKDROP_TYPE_PROCEDURALSUN;
            obj[BACKDROP_IS_DIRECTLY_VISIBLE]  = node.isDirectlyVisible();
            obj[BACKDROP_SUN_COLOR]            = toJson(node.sunColor());
            obj[BACKDROP_SKY_COLOR]            = toJson(node.skyColor());
            obj[BACKDROP_SKYLINE_COLOR]        = toJson(node.skylineColor());
            obj[BACKDROP_GROUND_COLOR]         = toJson(node.groundColor());
            obj[BACKDROP_GROUND_HEIGHT]        = node.groundHeight();
            obj[BACKDROP_SUN_DIR]              = toJson(node.sunDirection());
            _backdropsArray.append(obj);
        }
    }


    //////////////////////////
    // Surface Tree Builder //
    //////////////////////////
    StageSetJsonWriter::SurfaceTreeBuilder::SurfaceTreeBuilder(
            std::map<Surface*, int>& surfaceIdMap,
            std::map<Coating*, int>& coatingIdMap) :
        _surfaceIdMap(surfaceIdMap),
        _coatingIdMap(coatingIdMap)
    {

    }

    const QJsonValue& StageSetJsonWriter::SurfaceTreeBuilder::surfaceTree() const
    {
        return _subTree;
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(SurfaceShell& node)
    {
        auto children = node.children();
        assert(children.size() > 1);
        children[0]->accept(*this);

        QJsonObject logOpt;
        logOpt[SURFACE_OPERATOR_SHELL] = _subTree;
        logOpt[SURFACE_TRANSFORM] = toJson(node.transform());
        if(node.coating().get() != nullptr)
            logOpt[SURFACE_COATING] = _coatingIdMap[node.coating().get()];

        _subTree = logOpt;
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(SurfaceGhost& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject logOpt;
        logOpt[SURFACE_OPERATOR_GHOST] = _subTree;
        _subTree = logOpt;
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(SurfaceInverse& node)
    {
        auto children = node.children();
        assert(children.size() == 1);
        children[0]->accept(*this);

        QJsonObject logOpt;
        logOpt[SURFACE_OPERATOR_INVERSE] = _subTree;
        _subTree = logOpt;
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(SurfaceOr& node)
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

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(SurfaceAnd& node)
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

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(Box& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(Plane& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(PlaneTexture& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(Quadric& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }

    void StageSetJsonWriter::SurfaceTreeBuilder::visit(Sphere& node)
    {
        _subTree = QJsonValue(_surfaceIdMap[&node]);
    }
}
