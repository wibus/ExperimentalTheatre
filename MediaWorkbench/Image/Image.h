#ifndef MEDIAWORKBENCH_IMAGE_H
#define MEDIAWORKBENCH_IMAGE_H

#include <string>

#include "../libMediaWorkbench_global.h"


namespace media
{
    class MEDIA_EXPORT Image
    {
    public :
        enum class EFormat {GRAY, RGB, RGBA};

        Image();
        Image(const Image& image);
        Image(const std::string& fileName);
        Image(unsigned char *pixels,
              int width,
              int height);
        virtual ~Image();

        virtual Image& operator=(const Image& image);

        virtual bool    load(const std::string& fileName);
        virtual bool    save(const std::string& fileName) const;
        virtual Image&  addAlphaColor(unsigned char r,
                                      unsigned char g,
                                      unsigned char b);
        int id() const;
        virtual unsigned char*        pixels();
        virtual const unsigned char*  pixels() const;
        virtual unsigned char*        pixel(int x, int y);
        virtual const unsigned char*  pixel(int x, int y) const;
        virtual int indexOfPixel(int x, int y) const;

        virtual void setColor(int x, int y,
                              unsigned char r,
                              unsigned char g,
                              unsigned char b,
                              unsigned char a = 0xff);
        virtual void setColor(int x, int y,
                              unsigned int color);

        virtual int width() const;
        virtual int height() const;
        virtual int nbPixels() const;
        virtual int dataSize() const;
        virtual int pixelSize() const;
        virtual void clear();

        void toGray();
        void toRGB();

    private :
        int _id;
        unsigned char* _pixels;
        int _width;
        int _height;

        static int __attribId();
        static int __nextId;
    };



    // IMPLEMENTATION //
    inline int Image::id() const
        {return _id;}
}

#endif // MEDIAWORKBENCH_IMAGE_H
