#include "UniformStdMaterial.h"

#include "Node/Visitor.h"


namespace prop3
{
    UniformStdMaterial::UniformStdMaterial() :
        _opacity(1.0),
        _conductivity(0.0),
        _refractiveIndex(1.40),
        _scattering(1.0),
        _color(1.0)
    {

    }

    UniformStdMaterial::~UniformStdMaterial()
    {

    }

    void UniformStdMaterial::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void UniformStdMaterial::setOpacity(double translucency)
    {
        _opacity = translucency;
    }

    double UniformStdMaterial::opacity(const glm::dvec3& pos) const
    {
        return _opacity;
    }

    void UniformStdMaterial::setConductivity(double conductivity)
    {
        _conductivity = conductivity;
    }

    double UniformStdMaterial::conductivity(const glm::dvec3& pos) const
    {
        return _conductivity;
    }

    void UniformStdMaterial::setRefractiveIndex(double refractiveIndex)
    {
        _refractiveIndex = refractiveIndex;
    }

    double UniformStdMaterial::refractiveIndex(const glm::dvec3& pos) const
    {
        return _refractiveIndex;
    }

    void UniformStdMaterial::setScattering(double scattering)
    {
        _scattering = scattering;
    }

    double UniformStdMaterial::scattering(const glm::dvec3& pos) const
    {
        return _scattering;
    }

    void UniformStdMaterial::setColor(const glm::dvec3& color)
    {
        _color = color;
    }

    glm::dvec3 UniformStdMaterial::color(const glm::dvec3& pos) const
    {
        return _color;
    }
}
