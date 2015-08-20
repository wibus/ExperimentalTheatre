#ifndef PROPROOM3D_SCENEWRITER_H
#define PROPROOM3D_SCENEWRITER_H

#include <map>
#include <string>
#include <memory>

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <GLM/glm.hpp>

#include "SceneVisitor.h"


namespace prop3
{
    class Scene;
    class AbstractTeam;

    class PROP3D_EXPORT SceneWriter : public SceneVisitor
    {
    public :
        SceneWriter();
        virtual ~SceneWriter();

        virtual std::string write(Scene& scene, bool prettyPrint);


        // Props
        virtual void visit(Prop& node) override;

        // Implicit Surfaces
        virtual void visit(SurfaceGhost& node) override;
        virtual void visit(SurfaceInverse& node) override;
        virtual void visit(SurfaceOr& node) override;
        virtual void visit(SurfaceAnd& node) override;
        virtual void visit(Plane& node) override;
        virtual void visit(PlaneTexture& node) override;
        virtual void visit(Quadric& node) override;
        virtual void visit(Sphere& node) override;

        // Materials
        virtual void visit(Air& node) override;
        virtual void visit(Concrete& node) override;
        virtual void visit(Glass& node) override;
        virtual void visit(Metal& node) override;

        // Coatings
        virtual void visit(NoCoating& node) override;
        virtual void visit(FlatPaint& node) override;
        virtual void visit(GlossyPaint& node) override;
        virtual void visit(TexturedFlatPaint& node) override;
        virtual void visit(TexturedGlossyPaint& node) override;

    protected:
        static QJsonValue toJson(const glm::dvec3& v);
        static QJsonValue toJson(const glm::dvec4& v);
        static QJsonValue toJson(const glm::dmat4& m);

    private:
        bool insertProp(Prop& node);
        bool insertSurface(ImplicitSurface& node);
        bool insertMaterial(Material& node);
        bool insertCoating(Coating& node);

        std::map<ImplicitSurface*, int> _surfaceIdMap;
        std::map<Material*, int> _materialIdMap;
        std::map<Coating*, int> _coatingIdMap;
        std::map<Prop*, int> _propIdMap;

        QJsonArray _surfacesArray;
        QJsonArray _materialsArray;
        QJsonArray _coatingsArray;
        QJsonArray _propsArray;

        bool _surfaceTreeMode;
        QJsonValue _subTree;
    };
}

#endif // PROPROOM3D_SCENEWRITER_H
