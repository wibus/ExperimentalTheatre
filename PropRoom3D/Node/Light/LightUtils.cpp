#include "LightUtils.h"

#include <CellarWorkbench/Misc/FastMath.h>


namespace prop3
{
    glm::dvec3 kelvinToRgb(int kelvin)
    {
        glm::dvec3 color;
        double scaledKelvin = kelvin / 100.0;

        if(kelvin < 6600)
        {
            // Red
            color.r = 1.0;

            // Green
            double tmpG = scaledKelvin;
            tmpG = 0.390081579 * cellar::fast_log(tmpG) - 0.631841444;
            color.g = glm::clamp(tmpG, 0.0, 1.0);

            // Blue
            double tmpB = scaledKelvin - 10;
            tmpB = 0.543206789 * cellar::fast_log(tmpB) - 1.196254089;
            color.b = glm::clamp(tmpB, 0.0, 1.0);
        }
        else
        {
            // Red
            double tpmR = scaledKelvin - 60.0;
            tpmR = 1.292936186 * cellar::fast_pow(tpmR, -0.1332047592);
            color.r = glm::clamp(tpmR, 0.0, 1.0);

            // Green
            double tmpG = scaledKelvin - 60.0;
            tmpG = 1.129890861 * cellar::fast_pow(tmpG, -0.0755148492);
            color.g = glm::clamp(tmpG, 0.0, 1.0);

            // Blue
            color.b = 1.0;
        }

        return color;
    }
}
