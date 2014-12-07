#ifndef PROPROOM3D_ABSTRACTCOSTUME_H
#define PROPROOM3D_ABSTRACTCOSTUME_H

#include "libPropRoom3D_global.h"

#include <string>

#include <glm/glm.hpp>


namespace prop3
{
    class PROP3D_EXPORT AbstractCostume
    {
    protected:
        AbstractCostume();

    public:
        virtual ~AbstractCostume();

        // Getters
        glm::vec4 colorFilter() const;
        std::string textureName() const;

        // Setters
        virtual void setColorFilter(const glm::vec4& filter);
        virtual void setTextureName(const std::string& fileName);

    private:
        glm::vec4 _colorFilter;
        std::string _textureName;
    };



    // IMPLEMENTATION //

    inline glm::vec4 AbstractCostume::colorFilter() const
    {
        return _colorFilter;
    }

    inline std::string AbstractCostume::textureName() const
    {
        return _textureName;
    }
}

#endif // PROPROOM3D_ABSTRACTCOSTUME_H
