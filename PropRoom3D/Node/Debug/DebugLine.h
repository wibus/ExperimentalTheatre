#ifndef PROPROOM3D_DEBUGLINE_H
#define PROPROOM3D_DEBUGLINE_H

#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class PROP3D_EXPORT DebugLine
    {
    public:
        DebugLine(const glm::dvec3& color = glm::dvec3(1.0));
        ~DebugLine();

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
    inline const std::vector<glm::dvec3>& DebugLine::vertices() const
    {
        return _vertices;
    }

    inline glm::dvec3 DebugLine::color() const
    {
        return _color;
    }
}

#endif // PROPROOM3D_DEBUGLINE_H
