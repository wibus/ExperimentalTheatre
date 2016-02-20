#include "DebugLine.h"


namespace prop3
{
    DebugLine::DebugLine(const glm::dvec3 &color) :
        _color(color)
    {

    }

    DebugLine::~DebugLine()
    {

    }

    void DebugLine::clearVertices()
    {
        _vertices.clear();
    }

    void DebugLine::addVertex(const glm::dvec3& position)
    {
        _vertices.push_back(position);
    }

    void DebugLine::setVertices(const std::vector<glm::dvec3>& vertices)
    {
        _vertices = vertices;
    }

    void DebugLine::setColor(const glm::dvec3& color)
    {
        _color = color;
    }
}
