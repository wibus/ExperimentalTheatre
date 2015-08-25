#ifndef PROPROOM3D_STAGESETJSONREADER_H
#define PROPROOM3D_STAGESETJSONREADER_H

#include <string>
#include <memory>
#include <vector>

#include <QJsonObject>

#include <GLM/glm.hpp>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class AbstractTeam;

    class Surface;
    class Material;
    class Coating;

    class Backdrop;


    class PROP3D_EXPORT StageSetJsonReader
    {
    public :
        StageSetJsonReader();
        virtual ~StageSetJsonReader();

        virtual bool deserialize(AbstractTeam& team, const std::string& stream, bool clearStageSet = true);


    protected:
        static glm::dvec3 dvec3FromJson(const QJsonValueRef& ref);
        static glm::dvec4 dvec4FromJson(const QJsonValueRef& ref);
        static glm::dmat4 dmat4FromJson(const QJsonValueRef& ref);

    private:
        void deserializeBackdrops(const QJsonObject& stageSetObj);
        void deserializeEnvironment(const QJsonObject& stageSetObj, AbstractTeam& team);

        void deserializeCoatings(const QJsonObject& stageSetObj);
        void deserializeMaterials(const QJsonObject& stageSetObj);
        void deserializeSurfaces(const QJsonObject& stageSetObj);
        void deserializeProps(const QJsonObject& stageSetObj, AbstractTeam& team);

        std::shared_ptr<Surface> subSurfTree(
                const QJsonValue& surfaceTree);

        std::vector<std::shared_ptr<Backdrop>> _backdrops;
        std::vector<std::shared_ptr<Surface>> _surfaces;
        std::vector<std::shared_ptr<Material>> _materials;
        std::vector<std::shared_ptr<Coating>> _coatings;
    };
}

#endif // PROPROOM3D_STAGESETJSONREADER_H
