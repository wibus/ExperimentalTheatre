#ifndef MEDIAWORKBENCH_GLTOOLKIT_H
#define MEDIAWORKBENCH_GLTOOLKIT_H

#include <map>

#include "../libMediaWorkbench_global.h"


namespace media
{
    class Image;

    class MEDIA_EXPORT GlToolkit
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

#endif // MEDIAWORKBENCH_GLTOOLKIT_H
