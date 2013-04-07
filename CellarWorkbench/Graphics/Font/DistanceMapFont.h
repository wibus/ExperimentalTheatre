#ifndef CELLAR_DISTANCEMAPFONT_H
#define CELLAR_DISTANCEMAPFONT_H

#include "libCellarWorkbench_global.h"

#include <string>


namespace cellar
{
    class CELLAR_EXPORT DistanceMapFont
    {
    public:
        DistanceMapFont();

        bool loadFont(const std::string& fontName);

    };
}

#endif // DISTANCEMAPFONT_H
