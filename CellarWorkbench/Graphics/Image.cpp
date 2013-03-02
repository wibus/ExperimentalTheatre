#include "Image.h"
#include "Misc/Log.h"

#include <cassert>

using namespace std;

#include <QImage>


namespace cellar
{
    int Image::__nextId = 0;

    int Image::__attribId()
    {
        return __nextId++;
    }


    Image::Image() :
        _id( __attribId() ),
        _pixels(nullptr),
        _width(0),
        _height(0)
    {
    }

    Image::Image(const string& fileName) :
        _id( __attribId() ),
        _pixels(0x0),
        _width(0),
        _height(0)
    {
        load(fileName);
    }

    Image::Image(unsigned char *pixels,
                 int width,
                 int height) :
        _id( __attribId() ),
        _pixels(pixels),
        _width(width),
        _height(height)
    {
    }

    Image::Image(const Image& image) :
       _id( __attribId() ),
       _pixels(nullptr),
       _width(image._width),
       _height(image._height)
    {
        int size = dataSize();
        _pixels = new unsigned char[size];
        memcpy(_pixels, image._pixels, size);
    }

    Image::~Image()
    {
        clear();
    }

    unsigned char* Image::pixels()
    {
            return _pixels;
    }

    const unsigned char* Image::pixels() const
    {
            return _pixels;
    }

    unsigned char* Image::pixel(int x, int y)
    {
        return &_pixels[indexOfPixel(x, y)];
    }

    const unsigned char* Image::pixel(int x, int y) const
    {
        return &_pixels[indexOfPixel(x, y)];
    }

    void Image::setColor(int x,
                         int y,
                         unsigned char r,
                         unsigned char g,
                         unsigned char b,
                         unsigned char a)
    {
        int idx = indexOfPixel(x, y);
        _pixels[idx]   = r;
        _pixels[idx+1] = g;
        _pixels[idx+2] = b;
        _pixels[idx+3] = a;
    }

    void Image::setColor(int x, int y,
                         unsigned int color)
    {
        int idx = indexOfPixel(x, y);
        _pixels[idx]   = (color & 0x000000ff);
        _pixels[idx+1] = (color & 0x0000ff00) >> 8;
        _pixels[idx+2] = (color & 0x00ff0000) >> 16;
        _pixels[idx+3] = (color & 0xff000000) >> 24;
    }

    int Image::indexOfPixel(int x, int y) const
    {
        assert(0 <= x && x < _width);
        assert(0 <= y && y < _height);
        return (y*_width  + x) * pixelSize();
    }

    Image& Image::operator=(const Image& image)
    {
        if(this != &image)
        {
            _width = image._width;
            _height = image._height;

            // Copy pixels
            if(_pixels != 0x0)
                    delete[] _pixels;
            _pixels = 0;

            int size = dataSize();
            _pixels = new unsigned char[size];
            memcpy(_pixels, image._pixels, size);
        }

        return *this;
    }

    void Image::clear()
    {
        if(_pixels != nullptr)
                delete[] _pixels;
        _pixels = nullptr;
        _width = 0;
        _height = 0;
    }

    bool Image::load(const string& fileName)
    {
        clear();

        QImage img;
        if(img.load(fileName.c_str()))
        {
            img.convertToFormat(QImage::Format_ARGB32);
            _width = img.width();
            _height = img.height();

            int size = dataSize();
            _pixels = new unsigned char[size];
            const uchar* bits =  img.bits();
            for(int j=0; j<_height; ++j)
            {
                for(int i=0; i<_width; ++i)
                {
                    int tp = (j*_width + i)*4;
                    int op = ((_height-j-1)*_width+i)*4;
                    _pixels[tp]   = bits[op+2];
                    _pixels[tp+1] = bits[op+1];
                    _pixels[tp+2] = bits[op];
                    _pixels[tp+3] = bits[op+3];
                }
            }
            return true;
        }

        return false;
    }

    bool Image::save(const std::string& fileName) const
    {
        int size = dataSize();
        unsigned char* tmp = new unsigned char[size];
        for(int i=0; i<size; i+=4)
        {
            tmp[i] = _pixels[i+3];
            tmp[i+1] = _pixels[i];
            tmp[i+2] = _pixels[i+1];
            tmp[i+3] = _pixels[i+2];
        }

        QImage img(tmp,
                   _width,
                   _height,
                   _width*pixelSize(),
                   QImage::Format_ARGB32);

        delete [] tmp;
        tmp = nullptr;

        return img.save(fileName.c_str());
    }

    Image& Image::addAlphaColor(unsigned char r,
                                unsigned char g,
                                unsigned char b)
    {
        int size = dataSize();
        for(int i=0; i < size; i+=4)
        {
            if(_pixels[i]   == r &&
               _pixels[i+1] == g &&
               _pixels[i+2] == b)
            {
                _pixels[i+3] = 0;
            }
        }

        return *this;
    }

    int Image::width() const
    {
        return _width;
    }

    int Image::height() const
    {
        return _height;
    }

    int Image::nbPixels() const
    {
        return _width * _height;
    }

    int Image::dataSize() const
    {
        return _width * _height * pixelSize();
    }

    int Image::pixelSize() const
    {
        return 4;
    }

    void Image::toGray()
    {
        int size = dataSize();
        for(int i=0; i < size; i+=4)
        {
            unsigned int mix = (_pixels[i] + _pixels[i+1] + _pixels[i+2]) / 3;
            _pixels[i]   = mix;
            _pixels[i+1] = mix;
            _pixels[i+2] = mix;
        }
    }

    void Image::toRGB()
    {
        int size = dataSize();
        for(int i=0; i < size; i+=4)
        {
            _pixels[i+3] = 0xff;
        }
    }
}
