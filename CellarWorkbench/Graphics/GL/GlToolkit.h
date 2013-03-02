#ifndef CELLARWORKBENCH_GLTOOLKIT_H
#define CELLARWORKBENCH_GLTOOLKIT_H

#include <map>

#include "libCellarWorkbench_global.h"


namespace cellar
{
    class Image;

    class CELLAR_EXPORT GlToolkit
    {
    public:
        GlToolkit();

        static unsigned int genTextureId(const Image& image);
        static unsigned int updateTextureId(const Image& image);
        static bool         deleteTextureId(unsigned int id);

    private:
        static std::map<unsigned int, unsigned int> _imagId_texId;
        typedef std::map<unsigned int, unsigned int>::iterator idIterator;
    };
}

#endif // CELLARWORKBENCH_GLTOOLKIT_H
