#ifndef PROPROOM3D_PROCEDURALSUN_H
#define PROPROOM3D_PROCEDURALSUN_H

#include "Backdrop.h"


namespace prop3
{
    class Material;


    class PROP3D_EXPORT ProceduralSun : public Backdrop
    {
    public:
        ProceduralSun(bool isDirectlyVisible = true);


        virtual void accept(Visitor& visitor) override;


        glm::dvec3 sunColor() const;

        void setSunColor(const glm::dvec3& color);

        glm::dvec3 skyColor() const;

        void setSkyColor(const glm::dvec3& color);

        glm::dvec3 skylineColor() const;

        void setSkylineColor(const glm::dvec3& color);

        glm::dvec3 groundColor() const;

        void setGroundColor(const glm::dvec3& color);


        double groundHeight() const;

        void setGroundHeight(double height);

        glm::dvec3 sunDirection() const;

        void setSunDirection(const glm::dvec3& dir);


        virtual glm::dvec3 raycast(const Raycast& ray, bool directView) const override;

        virtual std::vector<Raycast> fireRays(unsigned int count) const override;

        virtual std::vector<Raycast> fireOn(const glm::dvec3& pos, unsigned int count) const override;




    protected:
        static const double SUN_COS_DIMENSION;
        static const double SUN_SMOOTH_EDGE;
        static const glm::dvec3 SKY_UP;


        static const double SUN_SIN;
        static const glm::dvec3 RECEIVE_PLANE_POS;
        static const double RECEIVE_PLANE_RADIUS;

        static const double RADIATION_PLANE_DISTANCE;
        static const double RADIATION_PLANE_RADIUS;


    private:
        glm::dvec3 _sunColor;
        glm::dvec3 _skyColor;
        glm::dvec3 _skylineColor;
        glm::dvec3 _groundColor;

        glm::dvec3 _sunDirection;
        double _groundHeight;

        std::shared_ptr<Material> _spaceMaterial;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 ProceduralSun::sunColor() const
    {
        return _sunColor;
    }

    inline glm::dvec3 ProceduralSun::skyColor() const
    {
        return _skyColor;
    }

    inline glm::dvec3 ProceduralSun::skylineColor() const
    {
        return _skylineColor;
    }

    inline glm::dvec3 ProceduralSun::groundColor() const
    {
        return _groundColor;
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
