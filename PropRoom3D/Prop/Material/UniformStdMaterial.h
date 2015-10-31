#ifndef PROPROOM3D_UNIFORMSTDMATERIAL_H
#define PROPROOM3D_UNIFORMSTDMATERIAL_H

#include "StdMaterial.h"


namespace prop3
{
    class PROP3D_EXPORT UniformStdMaterial : public StdMaterial
    {
    public:
        UniformStdMaterial();
        virtual ~UniformStdMaterial();


        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        // OPTICAL PROPERTIES
        virtual double opacity(const glm::dvec3& pos) const override;
        void setOpacity(double opacity);
        double opacity() const;

        virtual double conductivity(const glm::dvec3& pos) const override;
        void setConductivity(double conductivity);
        double conductivity() const;

        virtual double refractiveIndex(const glm::dvec3& pos) const override;
        void setRefractiveIndex(double refractiveIndex);
        double refractiveIndex() const;

        virtual double scattering(const glm::dvec3& pos) const override;
        void setScattering(double scattering);
        double scattering() const;

        virtual glm::dvec3 color(const glm::dvec3& pos) const override;
        void setColor(const glm::dvec3& color);
        glm::dvec3 color() const;


    protected:
        // OPTICAL PROPERTIES
        double _opacity;
        double _conductivity;
        double _refractiveIndex;
        double _scattering;
        glm::dvec3 _color;
    };


    // IMPLEMENTATION //
    inline double UniformStdMaterial::opacity() const
    {
        return _opacity;
    }

    inline double UniformStdMaterial::conductivity() const
    {
        return _conductivity;
    }

    inline double UniformStdMaterial::refractiveIndex() const
    {
        return _refractiveIndex;
    }

    inline double UniformStdMaterial::scattering() const
    {
        return _scattering;
    }

    inline glm::dvec3 UniformStdMaterial::color() const
    {
        return _color;
    }
}

#endif // PROPROOM3D_UNIFORMSTDMATERIAL_H
