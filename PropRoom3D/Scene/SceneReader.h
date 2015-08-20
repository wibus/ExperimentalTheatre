#ifndef PROPROOM3D_SCENEREADER_H
#define PROPROOM3D_SCENEREADER_H

#include <string>
#include <memory>
#include <vector>

#include <QJsonObject>

#include <GLM/glm.hpp>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class Scene;
    class AbstractTeam;

    class Prop;
    class ImplicitSurface;
    class Material;
    class Coating;


    class PROP3D_EXPORT SceneReader
    {
    public :
        SceneReader();
        virtual ~SceneReader();

        virtual bool read(Scene& scene, const std::string& stream);


    protected:
        static glm::dvec3 dvec3FromJson(const QJsonValueRef& ref);
        static glm::dvec4 dvec4FromJson(const QJsonValueRef& ref);
        static glm::dmat4 dmat4FromJson(const QJsonValueRef& ref);

    private:
        void deserializeCoatings(const QJsonObject& sceneObj);
        void deserializeMaterials(const QJsonObject& sceneObj);
        void deserializeSurfaces(const QJsonObject& sceneObj);
        void deserializeProps(const QJsonObject& sceneObj, Scene& scene);
        std::shared_ptr<ImplicitSurface> subSurfTree(
                const QJsonValue& surfaceTree);

        std::vector<std::shared_ptr<ImplicitSurface>> _surfaces;
        std::vector<std::shared_ptr<Material>> _materials;
        std::vector<std::shared_ptr<Coating>> _coatings;
    };
}

#endif // PROPROOM3D_SCENEREADER_H
