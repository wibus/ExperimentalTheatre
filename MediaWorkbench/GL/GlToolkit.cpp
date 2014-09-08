#include "GlToolkit.h"
#include "DataStructure/VectorSpace.h"
#include "Misc/CellarUtils.h"
#include "Misc/Log.h"
#include "Image/Image.h"

#include <gl3w.h>

using namespace std;


namespace media
{
    std::map<unsigned int, unsigned int> GlToolkit::_imagId_texId =
            std::map<unsigned int, unsigned int>();

    GlToolkit::GlToolkit()
    {
    }

    unsigned int GlToolkit::genTextureId(const Image& image)
    {
        idIterator it = _imagId_texId.find(image.id());
        if(it != _imagId_texId.end())
            return it->second;

        GLuint id = 0;

        if(image.pixels() != 0x0)
        {
            glEnable(GL_TEXTURE_2D);
                glGenTextures(1,&id);
                glBindTexture(GL_TEXTURE_2D, id);

                    glTexImage2D(GL_TEXTURE_2D, 0, 4,
                                 image.width(), image.height(), 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE,
                                 image.pixels());

                    glTexParameteri(GL_TEXTURE_2D,
                                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,
                                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glDisable(GL_TEXTURE_2D);
        }

        _imagId_texId.insert(make_pair(image.id(), id));

        return id;
    }

    unsigned int GlToolkit::updateTextureId(const Image& image)
    {
        idIterator it = _imagId_texId.find(image.id());
        if(it == _imagId_texId.end())
            return genTextureId(image);

        unsigned int id = it->second;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, 4,
                     image.width(), image.height(), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     image.pixels());
        glDisable(GL_TEXTURE_2D);

        return id;
    }

    bool GlToolkit::deleteTextureId(unsigned int id)
    {
        for(idIterator it = _imagId_texId.begin(); it != _imagId_texId.end(); ++it)
        {
            if(it->second == id)
            {
                _imagId_texId.erase(it);
                break;
            }
        }

        if(!glIsTexture(id))
            return false;
        glDeleteTextures(1, &id);

        return true;
    }
}
