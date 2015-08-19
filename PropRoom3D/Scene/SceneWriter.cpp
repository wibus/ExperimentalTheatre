#include "SceneWriter.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

#include <QVariant>
#include <QVector3D>

#include "Scene.h"

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

    bool SceneWriter::write(
            const std::string& fileName,
            const std::shared_ptr<AbstractTeam>& team,
            Scene& scene)
    {
        // Scan scene
        _surfaceTreeMode = false;
        scene.makeTraveling(*this);

        // Write file
        QJsonObject sceneObj;
        sceneObj.insert("Materials", _materialsArray);
        sceneObj.insert("Coatings", _coatingsArray);
        sceneObj.insert("Surfaces", _surfacesArray);
        sceneObj.insert("Props", _propsArray);

        QJsonDocument doc;
        doc.setObject(sceneObj);

        QFile file(fileName.c_str());
        file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        file.write(doc.toJson());
        file.close();


        //Clean-up structures
        _surfaceIdMap.clear();
        _materialIdMap.clear();
        _coatingIdMap.clear();
        _propIdMap.clear();
    }

    QJsonValue SceneWriter::toJson(const glm::dvec3& v)
    {
        QJsonArray array;
        array.append(v.r);
        array.append(v.g);
        array.append(v.b);
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
                obj.insert("Type", QString("Prop"));

                obj.insert("Material", _materialIdMap[node.material().get()]);

                _surfaceTreeMode = true;
                if(node.surface().get() != nullptr)
                {
                    node.surface()->accept(*this);
                    obj.insert("Surface Tree", _subTree);
                }

                if(node.boundingSurface().get() != nullptr)
                {
                    node.boundingSurface()->accept(*this);
                    obj.insert("Bounding Surface Tree", _subTree);
                }
                _surfaceTreeMode = false;

                _propsArray.append(obj);
            }
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
                obj.insert("Type", QString("Air"));
                obj.insert("Refractive Index", node.refractiveIndex());
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
                obj.insert("Type", QString("Concrete"));
                obj.insert("Refractive Index", node.refractiveIndex());
                obj.insert("Color", toJson(node.color()));
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
                obj.insert("Type", QString("Glass"));
                obj.insert("Refractive Index", node.refractiveIndex());
                obj.insert("Color", toJson(node.color()));
                obj.insert("Dye Concentration", node.dyeConcentration());
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
                obj.insert("Type", QString("Metal"));
                obj.insert("Refractive Index", node.refractiveIndex());
                obj.insert("Color", toJson(node.color()));
                obj.insert("Glossiness", node.glossiness());
                _materialsArray.append(obj);
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
            logOpt.insert("~", _subTree);
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
            logOpt.insert("!", _subTree);
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
            localSubTree.insert("|", childArray);
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
            localSubTree.insert("&", childArray);
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
                obj.insert("Type", QString("Plane"));
                obj.insert("Distance", node.distance());
                obj.insert("Normal", toJson(node.normal()));
                obj.insert("Coating", _coatingIdMap[node.coating().get()]);
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
                obj.insert("Type", QString("PlaneTexture"));
                obj.insert("Distance", node.distance());
                obj.insert("Normal", toJson(node.normal()));
                obj.insert("Coating", _coatingIdMap[node.coating().get()]);
                obj.insert("Texure Origin", toJson(node.texOrigin()));
                obj.insert("Texuture U", toJson(node.texU()));
                obj.insert("Texuture V", toJson(node.texV()));
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
                obj.insert("Type", QString("Quadric"));
                obj.insert("Representation", toJson(node.matrixRepresentation()));
                obj.insert("Coating", _coatingIdMap[node.coating().get()]);
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
                obj.insert("Type", QString("Sphere"));
                obj.insert("Radius", node.radius());
                obj.insert("Center", toJson(node.center()));
                obj.insert("Coating", _coatingIdMap[node.coating().get()]);
                _surfacesArray.append(obj);
            }
        }
        else
        {
            _subTree = QJsonValue(_surfaceIdMap[&node]);
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
                obj.insert("Type", QString("NoCoating"));
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
                obj.insert("Type", QString("FlaPaint"));
                obj.insert("Color", toJson(node.color()));
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
                obj.insert("Type", QString("GlossyPaint"));
                obj.insert("Color", toJson(node.color()));
                obj.insert("Glossiness", node.glossiness());
                obj.insert("Varnish Refractive Index", node.varnishRefractiveIndex());
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
                obj.insert("Type", QString("TexturedFlatPaint"));
                obj.insert("Default Color", toJson(node.defaultColor()));
                obj.insert("Texture Name", QString(node.texName().c_str()));
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
                obj.insert("Type", QString("TexturedGlossyPaint"));
                obj.insert("Default Color", toJson(node.defaultColor()));
                obj.insert("Default Glossiness", node.defaultGlossiness());
                obj.insert("Texture Name", QString(node.texName().c_str()));
                obj.insert("GlossMap Name", QString(node.glossName().c_str()));
                obj.insert("Varnish Refractive Index", node.varnishRefractiveIndex());
                _coatingsArray.append(obj);
            }
        }
    }
}
