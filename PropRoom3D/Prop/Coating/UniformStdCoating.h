#ifndef PROPROOM3D_UNIFORMSTDCOATING_H
#define PROPROOM3D_UNIFORMSTDCOATING_H

#include "StdCoating.h"


namespace prop3
{

    class PROP3D_EXPORT UniformStdCoating : public StdCoating
    {
    public:
        UniformStdCoating();
        virtual ~UniformStdCoating();


        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual double roughness(const glm::dvec3& tex) const override;
        void setRoughness(double roughness);
        double roughness() const;

        virtual glm::dvec4 paintColor(const glm::dvec3& tex) const override;
        void setPaintColor(const glm::dvec4& color);
        glm::dvec4 paintColor() const;

        virtual double paintRefractiveIndex(const glm::dvec3& tex) const override;
        void setPaintRefractiveIndex(double refractiveIndex);
        double paintRefractiveIndex() const;


    protected:
        double _roughness;
        glm::dvec4 _paintColor;
        double _paintRefractiveIndex;
    };



    // IMPLEMENTATION //
    inline double UniformStdCoating::roughness() const
    {
        return _roughness;
    }

    inline glm::dvec4 UniformStdCoating::paintColor() const
    {
        return _paintColor;
    }

    inline double UniformStdCoating::paintRefractiveIndex() const
    {
        return _paintRefractiveIndex;
    }
}

#endif // PROPROOM3D_UNIFORMSTDCOATING_H
