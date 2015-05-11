#ifndef PROPROOM2D_ABSTRACTCOSTUME_H
#define PROPROOM2D_ABSTRACTCOSTUME_H

#include <string>

#include <GLM/glm.hpp>

#include "../../libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT AbstractCostume
    {
    protected:
        AbstractCostume();

    public:
        virtual ~AbstractCostume();

        // Getters
        double depth() const;
        glm::dvec4 colorFilter() const;
        std::string textureName() const;

        // Setters
        virtual void setDepth(double depth);
        virtual void setColorFilter(const glm::dvec4& filter);
        virtual void setTextureName(const std::string& fileName);

    private:
        double _depth;
        glm::dvec4 _colorFilter;
        std::string _textureName;
    };



    // IMPLEMENTATION //

    inline double AbstractCostume::depth() const
    {
        return _depth;
    }

    inline glm::dvec4 AbstractCostume::colorFilter() const
    {
        return _colorFilter;
    }

    inline std::string AbstractCostume::textureName() const
    {
        return _textureName;
    }
}

#endif // PROPROOM2D_ABSTRACTCOSTUME_H
