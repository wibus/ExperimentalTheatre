#include "Texture.h"

#include "Image.h"


namespace cellar
{
    double* Texture::_gammaLut = nullptr;

    Texture::Texture(const Image& image, bool linearize) :
        _width(image.width()),
        _height(image.height()),
        _pixels(image.nbPixels())
    {
        if(_gammaLut == nullptr)
        {
            _gammaLut = new double[256];
            for(int i=0; i < 256; ++i)
            {
                double x = i / 255.0;
                _gammaLut[i] = glm::pow(x, 2.2);
            }
        }

        size_t idx = 0;
        for(int j=0; j < image.height(); ++j)
        {
            for(int i=0; i < image.width(); ++i)
            {
                const unsigned char* pix = image.pixel(i, j);

                if(linearize)
                {
                    double r = _gammaLut[pix[0]];
                    double g = _gammaLut[pix[1]];
                    double b = _gammaLut[pix[2]];

                    _pixels[idx] = glm::vec3(r, g, b);
                }
                else
                {
                    double r = pix[0] / 255.0;
                    double g = pix[1] / 255.0;
                    double b = pix[2] / 255.0;

                    _pixels[idx] = glm::vec3(r, g, b);
                }

                ++idx;
            }
        }
    }
}
