#ifndef PROPROOM3D_PROCEDURALSUN_H
#define PROPROOM3D_PROCEDURALSUN_H

#include <CellarWorkbench/Misc/Distribution.h>

#include "Backdrop.h"


namespace prop3
{
    class Material;


    class PROP3D_EXPORT ProceduralSun : public Backdrop
    {
    public:
        ProceduralSun();


        virtual void accept(Visitor& visitor) override;


        double sunIntensity() const;

        void setSunIntensity(double intensity);

        glm::dvec3 skyColor() const;

        void setSkyColor(const glm::dvec3& color);


        double groundHeight() const;

        void setGroundHeight(double height);

        glm::dvec3 sunDirection() const;

        void setSunDirection(const glm::dvec3& dir);


        virtual double distance(const Raycast& ray) const override;

        virtual glm::dvec4 raycast(const Raycast& ray) const override;

        virtual void fireOn(
                std::vector<LightCast>& lightCasts,
                const glm::dvec3& pos,
                unsigned int count) const override;


        static const double SUN_COS_RADIUS;
        static const double SUN_SURFACE_RATIO;
        static const glm::dvec3 SKY_UP;

        static const double SUN_SIN;
        static const glm::dvec3 RECEIVE_PLANE_POS;
        static const double RECEIVE_PLANE_RADIUS;

        static const double RADIATION_PLANE_DISTANCE;
        static const double RADIATION_PLANE_RADIUS;


    private:
        double _sunIntensity;
        glm::dvec3 _skyColor;

        glm::dvec3 _sunDirection;
        glm::dvec3 _sunSideward;
        glm::dvec3 _sunUpward;

        glm::dvec3 _sunColor;
        glm::dvec3 _haloColor;
        glm::dvec3 _skylineColor;
        glm::dvec3 _groundColor;
        double _halowIntensity;
        double _diffuseIntens;

        double _groundHeight;

        std::shared_ptr<Material> _spaceMaterial;

        cellar::DiskRand _diskRand;
    };



    // IMPLEMENTATION //
    inline double ProceduralSun::sunIntensity() const
    {
        return _sunIntensity;
    }

    inline glm::dvec3 ProceduralSun::skyColor() const
    {
        return _skyColor;
    }

    inline double ProceduralSun::groundHeight() const
    {
        return _groundHeight;
    }

    inline glm::dvec3 ProceduralSun::sunDirection() const
    {
        return _sunDirection;
    }
}

#endif // PROPROOM3D_PROCEDURALSUN_H
