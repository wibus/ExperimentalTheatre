#ifndef PROPROOM3D_SCENEJSONWRITER_H
#define PROPROOM3D_SCENEJSONWRITER_H

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


    class PROP3D_EXPORT SceneJsonWriter : public SceneVisitor
    {
    private:
        class SurfaceTreeBuilder : public SceneVisitor
        {
        public:
            SurfaceTreeBuilder(std::map<ImplicitSurface*, int>& surfaceIdMap);

            const QJsonValue& surfaceTree() const;

            // Implicit Surfaces
            virtual void visit(SurfaceGhost& node) override;
            virtual void visit(SurfaceInverse& node) override;
            virtual void visit(SurfaceOr& node) override;
            virtual void visit(SurfaceAnd& node) override;
            virtual void visit(Plane& node) override;
            virtual void visit(PlaneTexture& node) override;
            virtual void visit(Quadric& node) override;
            virtual void visit(Sphere& node) override;

        private:
            QJsonValue _subTree;
            std::map<ImplicitSurface*, int>& _surfaceIdMap;
        };

    public :
        SceneJsonWriter();
        virtual ~SceneJsonWriter();

        virtual std::string serialize(Scene& scene, bool prettyPrint = false);

        virtual bool saveToFile(Scene& scene, const std::string& fileName, bool prettyPrint = true);


        // Props
        virtual void visit(Prop& node) override;

        // Implicit Surfaces
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
        bool insertSurface(ImplicitSurface& node);
        bool insertMaterial(Material& node);
        bool insertCoating(Coating& node);

        std::map<ImplicitSurface*, int> _surfaceIdMap;
        std::map<Material*, int> _materialIdMap;
        std::map<Coating*, int> _coatingIdMap;

        QJsonArray _surfacesArray;
        QJsonArray _materialsArray;
        QJsonArray _coatingsArray;
        QJsonArray _propsArray;
    };
}

#endif // PROPROOM3D_SCENEJSONWRITER_H
