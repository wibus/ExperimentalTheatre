#ifndef PROPROOM3D_DEBUGPOINTCLOUND_H
#define PROPROOM3D_DEBUGPOINTCLOUND_H

#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class PROP3D_EXPORT DebugPointCloud
    {
    public:
        DebugPointCloud(const glm::dvec3& color = glm::dvec3(1.0));
        ~DebugPointCloud();

        void clearVertices();
        void addVertex(const glm::dvec3& position);
        void setVertices(const std::vector<glm::dvec3>& vertices);
        const std::vector<glm::dvec3>& vertices() const;

        void setColor(const glm::dvec3& color);
        glm::dvec3 color() const;

    private:
        glm::dvec3 _color;
        std::vector<glm::dvec3> _vertices;
    };



    // IMPLEMENTATION //
    inline const std::vector<glm::dvec3>& DebugPointCloud::vertices() const
    {
        return _vertices;
    }

    inline glm::dvec3 DebugPointCloud::color() const
    {
        return _color;
    }
}

#endif // PROPROOM3D_DEBUGPOINTCLOUND_H
