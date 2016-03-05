#include "DebugLineStrip.h"


namespace prop3
{
    DebugLineStrip::DebugLineStrip(const glm::dvec3 &color) :
        _color(color)
    {

    }

    DebugLineStrip::~DebugLineStrip()
    {

    }

    void DebugLineStrip::clearVertices()
    {
        _vertices.clear();
    }

    void DebugLineStrip::addVertex(const glm::dvec3& position)
    {
        _vertices.push_back(position);
    }

    void DebugLineStrip::setVertices(const std::vector<glm::dvec3>& vertices)
    {
        _vertices = vertices;
    }

    void DebugLineStrip::setColor(const glm::dvec3& color)
    {
        _color = color;
    }
}
