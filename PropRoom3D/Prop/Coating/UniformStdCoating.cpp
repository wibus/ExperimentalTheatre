#include "UniformStdCoating.h"

#include "Serial/Visitor.h"


namespace prop3
{
    UniformStdCoating::UniformStdCoating() :
        _roughness(0.0),
        _paintColor(1, 1, 1, 0),
        _paintRefractiveIndex(1.55)
    {
    }

    UniformStdCoating::~UniformStdCoating()
    {

    }

    void UniformStdCoating::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    double UniformStdCoating::roughness(const glm::dvec3& tex) const
    {
        return _roughness;
    }

    void UniformStdCoating::setRoughness(double roughness)
    {
        _roughness = roughness;
    }

    glm::dvec4 UniformStdCoating::paintColor(const glm::dvec3& tex) const
    {
        return _paintColor;
    }

    void UniformStdCoating::setPaintColor(const glm::dvec4& color)
    {
        _paintColor = color;
    }

    double UniformStdCoating::paintRefractiveIndex(const glm::dvec3& tex) const
    {
        return _paintRefractiveIndex;
    }

    void UniformStdCoating::setPaintRefractiveIndex(double refractiveIndex)
    {
        _paintRefractiveIndex = refractiveIndex;
    }
}
