#ifdef FFMPEG_ENABLED
#ifndef CELLARWORKBENCH_GLFFMPEGCAMCORDER_H
#define CELLARWORKBENCH_GLFFMPEGCAMCORDER_H

#include <string>
#include <fstream>

extern "C"
{
    struct AVCodec;
    struct AVCodecContext;
    struct AVFrame;
    struct AVPacket;
    struct SwsContext;
}


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT GLFFmpegCamcorder
    {
    public:
        GLFFmpegCamcorder();
        ~GLFFmpegCamcorder();

        void setFileName(const std::string& fileName);
        void setFrameRate(int fps);
        void setBitRate(int br);
        void setFrameWidth(int width);
        void setFrameHeight(int height);

        bool setup();
        void recordFrame();
        bool finalise();

    private:
        std::string _fileName;
        int _framePts;
        int _frameRate;
        int _bitRate;
        int _frameWidth;
        int _frameHeight;

        std::ofstream _videoStream;
        unsigned char* _frameBuff;

        AVCodec* _codec;
        AVCodecContext* _context;
        AVFrame* _srcFrame;
        AVFrame* _dstFrame;
        SwsContext* _swsContext;
        AVPacket* _paket;
        int _avcodecId; // AVCodecID
    };



    // IMPLEMENTATION //
    inline void GLFFmpegCamcorder::setFileName(const std::string& fileName)
    {
        _fileName = fileName;
    }

    inline void GLFFmpegCamcorder::setFrameRate(int fps)
    {
        _frameRate = fps;
    }

    inline void GLFFmpegCamcorder::setBitRate(int br)
    {
        _bitRate = br;
    }

    inline void GLFFmpegCamcorder::setFrameWidth(int width)
    {
        _frameWidth = width;
    }

    inline void GLFFmpegCamcorder::setFrameHeight(int height)
    {
        _frameHeight = height;
    }
}

#endif // CELLARWORKBENCH_GLFFMPEGCAMCORDER_H
#endif //FFMPEG_ENABLED
