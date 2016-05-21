#ifndef PROPROOM3D_LIGHTUTILS_H
#define PROPROOM3D_LIGHTUTILS_H

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    //
    // Utilities
    //

    // Kevin degree to RGB color value
    // Perfect white is 6600K
    PROP3D_EXPORT glm::dvec3 kelvinToRgb(int kelvin);

    PROP3D_EXPORT double luminance(const glm::dvec3& color);
}

#endif // PROPROOM3D_LIGHTUTILS_H
