#ifndef CELLARWORKBENCH_TEXTURE_H
#define CELLARWORKBENCH_TEXTURE_H

#include <vector>

#include <GLM/glm.hpp>


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class Image;


    class CELLAR_EXPORT Texture
    {
    public :
        Texture(const Image& img, bool linearize);

        int width() const;
        int height() const;

        glm::vec3& at(int i, int j);
        const glm::vec3& at(int i, int j) const;

    private :
        int _width;
        int _height;
        std::vector<glm::vec3> _pixels;

        static double* _gammaLut;
    };



    // IMPLEMENTATION //
    inline int Texture::width() const
    {
        return _width;
    }

    inline int Texture::height() const
    {
        return _height;
    }

    inline glm::vec3& Texture::at(int i, int j)
    {
        return _pixels[j*_width + i];
    }

    inline const glm::vec3& Texture::at(int i, int j) const
    {
        return _pixels[j*_width + i];
    }
}

#endif // CELLARWORKBENCH_TEXTURE_H
