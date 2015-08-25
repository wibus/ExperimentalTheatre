#ifndef PROPROOM3D_PROCEDURALSUN_H
#define PROPROOM3D_PROCEDURALSUN_H

#include "Backdrop.h"


namespace prop3
{
    class PROP3D_EXPORT ProceduralSun : public Backdrop
    {
    public:
        ProceduralSun(bool isDirectlyVisible = true);


        glm::dvec3 sunColor() const;

        void setSunColor(const glm::dvec3& color);

        glm::dvec3 skyColor() const;

        void setSkyColor(const glm::dvec3& color);

        glm::dvec3 skylineColor() const;

        void setSkylineColor(const glm::dvec3& color);


        glm::dvec3 upSkyDirection() const;

        void setUpSkyDirection(const glm::dvec3& dir);

        glm::dvec3 sunDirection() const;

        void setSunDirection(const glm::dvec3& dir);


        virtual glm::dvec3 raycast(const Ray& ray) const override;


        virtual void accept(StageSetVisitor& visito) override;

    private:
        glm::dvec3 _sunColor;
        glm::dvec3 _skyColor;
        glm::dvec3 _skylineColor;

        glm::dvec3 _upSkyDirection;
        glm::dvec3 _sunDirection;
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

    inline glm::dvec3 ProceduralSun::upSkyDirection() const
    {
        return _upSkyDirection;
    }

    inline glm::dvec3 ProceduralSun::sunDirection() const
    {
        return _sunDirection;
    }
}

#endif // PROPROOM3D_PROCEDURALSUN_H
