#ifndef CELLARWORKBENCH_FASTMATH_H
#define CELLARWORKBENCH_FASTMATH_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    CELLAR_EXPORT float fast_log2(float val);
    CELLAR_EXPORT inline float fast_log (const float &val)
    {
       return (fast_log2(val) * 0.69314718f);
    }

    CELLAR_EXPORT float fast_pow(float x, float y);
    CELLAR_EXPORT glm::vec2 fast_pow(const glm::vec2& x, const glm::vec2& y);
    CELLAR_EXPORT glm::vec3 fast_pow(const glm::vec3& x, const glm::vec3& y);
    CELLAR_EXPORT glm::vec4 fast_pow(const glm::vec4& x, const glm::vec4& y);
}

#endif // CELLARWORKBENCH_FASTMATH_H
