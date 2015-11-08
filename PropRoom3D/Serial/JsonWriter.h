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

#include "Serial/Visitor.h"


namespace prop3
{
    class StageSet;
    class HandleNode;
    class PhysicalSurface;


    class PROP3D_EXPORT StageSetJsonWriter
    {
    public :
        StageSetJsonWriter();
        virtual ~StageSetJsonWriter();

        virtual std::string serialize(StageSet& stageSet, bool prettyPrint = false);


    private:
        static QJsonValue toJson(const glm::dvec3& v);
        static QJsonValue toJson(const glm::dvec4& v);
        static QJsonValue toJson(const glm::dmat4& m);
        static QJsonValue toJson(const cellar::ESamplerFilter& filter);
        static QJsonValue toJson(const cellar::ESamplerWrapper& wrapper);

        class HardwareBuilder : public Visitor
        {
        public:

            // Surfaces
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


            QJsonArray surfacesArray;
            QJsonArray coatingsArray;
            QJsonArray materialsArray;

            std::map<Surface*,  int> surfaceIdMap;
            std::map<Coating*,  int> coatingIdMap;
            std::map<Material*, int> materialIdMap;

        private:
            bool insertSurface(Surface& node);
            bool insertMaterial(Material& node);
            bool insertCoating(Coating& node);
            void setPhysicalProperties(PhysicalSurface& node, QJsonObject& obj);
        };

        class StageSetBuilder : public Visitor
        {
        public:
            StageSetBuilder(std::map<Surface*,  int>& surfaceIdMap,
                            std::map<Coating*,  int>& coatingIdMap,
                            std::map<Material*, int>& materialIdMap);

            // Stage set
            virtual void visit(StageSet& node) override;

            // Zones
            virtual void visit(StageZone& node) override;

            // Props
            virtual void visit(Prop& node) override;

            // Environment
            virtual void visit(Environment& node) override;

            // Backdrop
            virtual void visit(ProceduralSun& node) override;

            // Surfaces
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

            QJsonValue subTree;

        private:
            void setHandleNodeProperties(HandleNode& node, QJsonObject& obj);

            std::map<Surface*,  int>& _surfaceIdMap;
            std::map<Coating*,  int>& _coatingIdMap;
            std::map<Material*, int>& _materialIdMap;
        };
    };
}

#endif // PROPROOM3D_STAGESETJSONWRITER_H
