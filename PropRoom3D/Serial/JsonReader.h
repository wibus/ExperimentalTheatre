#ifndef PROPROOM3D_STAGESETJSONREADER_H
#define PROPROOM3D_STAGESETJSONREADER_H

#include <string>
#include <memory>
#include <vector>

#include <QJsonObject>

#include <GLM/glm.hpp>

#include <CellarWorkbench/Image/ImageSampler.h>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class AbstractTeam;

    class HandleNode;
    class StageSet;
    class StageZone;

    class Prop;

    class Surface;
    class Material;
    class Coating;

    class Light;
    class Sampler;

    class Environment;
    class Backdrop;


    class PROP3D_EXPORT StageSetJsonReader
    {
    public :
        StageSetJsonReader();
        virtual ~StageSetJsonReader();

        virtual bool deserialize(AbstractTeam& team, const std::string& stream);


    protected:
        static glm::dvec3 dvec3FromJson(const QJsonValueRef& ref);
        static glm::dvec4 dvec4FromJson(const QJsonValueRef& ref);
        static glm::dmat4 dmat4FromJson(const QJsonValueRef& ref);
        static cellar::ESamplerFilter  filterFromJson(const QJsonValueRef& ref);
        static cellar::ESamplerWrapper wrapperFromJson(const QJsonValueRef& ref);

    private:
        void deserializeSamplers(const QJsonObject& docObj);
        void deserializeLights(const QJsonObject& docObj);
        void deserializeMaterials(const QJsonObject& docObj);
        void deserializeCoatings(const QJsonObject& docObj);
        void deserializeSurfaces(const QJsonObject& docObj);

        void deserializeStageSet(const QJsonValueRef& ref, AbstractTeam& team);
        std::shared_ptr<StageZone> deserializeZone(const QJsonValueRef& ref, AbstractTeam& team);
        std::shared_ptr<Prop> deserializeProp(const QJsonValueRef& ref, AbstractTeam& team);
        std::shared_ptr<Environment> deserializeEnvironment(const QJsonValueRef& ref, AbstractTeam& team);
        std::shared_ptr<Backdrop> deserializeBackdrop(const QJsonValueRef& ref, AbstractTeam& team);

        std::shared_ptr<Surface> subSurfTree(
                const QJsonValue& surfaceTree);
        void setHandleNodeProperties(
                HandleNode& node,
                const QJsonObject& obj);
        void setStageZoneProperties(
                StageZone& node,
                const QJsonObject& obj,
                AbstractTeam& team);


        std::vector<std::shared_ptr<Sampler>>   _samplers;
        std::vector<std::shared_ptr<Light>>     _lights;
        std::vector<std::shared_ptr<Material>>  _materials;
        std::vector<std::shared_ptr<Coating>>   _coatings;
        std::vector<std::shared_ptr<Surface>>   _surfaces;
    };
}

#endif // PROPROOM3D_STAGESETJSONREADER_H
