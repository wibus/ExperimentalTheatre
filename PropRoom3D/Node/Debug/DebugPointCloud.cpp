#include "DebugPointCloud.h"


namespace prop3
{
    DebugPointCloud::DebugPointCloud(const glm::dvec3 &color) :
        _color(color)
    {

    }

    DebugPointCloud::~DebugPointCloud()
    {

    }

    void DebugPointCloud::clearVertices()
    {
        _vertices.clear();
    }

    void DebugPointCloud::addVertex(const glm::dvec3& position)
    {
        _vertices.push_back(position);
    }

    void DebugPointCloud::setVertices(const std::vector<glm::dvec3>& vertices)
    {
        _vertices = vertices;
    }

    void DebugPointCloud::setColor(const glm::dvec3& color)
    {
        _color = color;
    }
}
