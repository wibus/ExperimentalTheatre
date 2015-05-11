#ifndef PROPROOM2D_CIRCLECOSTUME_H
#define PROPROOM2D_CIRCLECOSTUME_H

#include "AbstractCostume.h"


namespace prop2
{
    class PROP2D_EXPORT CircleCostume : public AbstractCostume
    {
    public:
        CircleCostume();
        virtual ~CircleCostume();

        // Getters
        double textureRadius() const;
        glm::dvec2 textureCenter() const;

        // Setters
        virtual void setTextureRadius(double radius);
        virtual void setTextureCenter(const glm::dvec2 center);

    private:
        double _textureRadius;
        glm::dvec2 _textureCenter;
    };



    // IMPLEMENTATION //
    inline double CircleCostume::textureRadius() const
    {
        return _textureRadius;
    }

    inline glm::dvec2 CircleCostume::textureCenter() const
    {
        return _textureCenter;
    }
}

#endif // PROPROOM2D_CIRCLECOSTUME_H
