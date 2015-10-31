#ifndef PROPROOM3D_STAGESETJSONWRITER_H
#define PROPROOM3D_STAGESETJSONWRITER_H

#include <map>
#include <string>
#include <memory>

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include <GLM/glm.hpp>

#include <CellarWorkbench/Image/ImageSampler.h>

#include "StageSetVisitor.h"


namespace prop3
{
    class StageSet;
    class PhysicalSurface;


    class PROP3D_EXPORT StageSetJsonWriter : public StageSetVisitor
    {
    private:
        class SurfaceTreeBuilder : public StageSetVisitor
        {
        public:
            SurfaceTreeBuilder(std::map<Surface*, int>&  surfaceIdMap,
                               std::map<Coating*, int>&  coatingIdMap,
                               std::map<Material*, int>& materialIdMap);

            const QJsonValue& surfaceTree() const;

            // Implicit Surfaces
            virtual void visit(SurfaceShell& node) override;
            virtual void visit(SurfaceGhost& node) override;
            virtual void visit(SurfaceInverse& node) override;
            virtual void visit(SurfaceOr& node) override;
            virtual void visit(SurfaceAnd& node) override;
            virtual void visit(Box& node) override;
            virtual void visit(BoxTexture& node) override;
            virtual void visit(Plane& node) override;
            virtual void visit(PlaneTexture& node) override;
            virtual void visit(Quadric& node) override;
            virtual void visit(Sphere& node) override;

        private:
            QJsonValue _subTree;
            std::map<Surface*, int>&  _surfaceIdMap;
            std::map<Coating*, int>&  _coatingIdMap;
            std::map<Material*, int>& _materialIdMap;
        };

    public :
        StageSetJsonWriter();
        virtual ~StageSetJsonWriter();

        virtual std::string serialize(StageSet& stageSet, bool prettyPrint = false);


        // Props
        virtual void visit(Prop& node) override;

        // Implicit Surfaces
        virtual void visit(Box& node) override;
        virtual void visit(BoxTexture& node) override;
        virtual void visit(Plane& node) override;
        virtual void visit(PlaneTexture& node) override;
        virtual void visit(Quadric& node) override;
        virtual void visit(Sphere& node) override;

        // Materials
        virtual void visit(UniformStdMaterial& node);

        // Coatings
        virtual void visit(UniformStdCoating& node);
        virtual void visit(TexturedStdCoating& node);

        // Environments
        virtual void visit(Environment& node) override;

        // Backdrops
        virtual void visit(ProceduralSun& node) override;

    protected:
        static QJsonValue toJson(const glm::dvec3& v);
        static QJsonValue toJson(const glm::dvec4& v);
        static QJsonValue toJson(const glm::dmat4& m);
        static QJsonValue toJson(const cellar::ESamplerFilter& filter);
        static QJsonValue toJson(const cellar::ESamplerWrapper& wrapper);

    private:
        bool insertBackdrop(Backdrop& node);
        bool insertSurface(Surface& node);
        bool insertMaterial(Material& node);
        bool insertCoating(Coating& node);

        void addPhysicalProperties(PhysicalSurface& node, QJsonObject& obj);

        std::map<Backdrop*, int> _backdropIdMap;
        std::map<Coating*, int> _coatingIdMap;
        std::map<Material*, int> _materialIdMap;
        std::map<Surface*, int> _surfaceIdMap;

        QJsonObject _environmentObject;
        QJsonArray _backdropsArray;
        QJsonArray _surfacesArray;
        QJsonArray _materialsArray;
        QJsonArray _coatingsArray;
        QJsonArray _propsArray;
    };
}

#endif // PROPROOM3D_STAGESETJSONWRITER_H
