#ifdef FFMPEG_ENABLED

#include "GLFFmpegCamcorder.h"

#include <gl3w.h>

extern "C"
{
    #include <libavutil/opt.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/audioconvert.h>
    #include <libavutil/common.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/samplefmt.h>
    #include <libswscale/swscale.h>
}

#include <CellarWorkbench/Misc/Log.h>


using namespace std;

namespace media
{
    GLFFmpegCamcorder::GLFFmpegCamcorder() :
        _fileName(),
        _framePts(0),
        _frameRate(24),
        _bitRate(250000),
        _frameWidth(600),
        _frameHeight(400),
        _videoStream(),
        _frameBuff(nullptr),
        _codec(nullptr),
        _context(nullptr),
        _srcFrame(nullptr),
        _dstFrame(nullptr),
        _swsContext(nullptr),
        _paket(new AVPacket()),
        _avcodecId(0)
    {
        avcodec_register_all();
    }

    GLFFmpegCamcorder::~GLFFmpegCamcorder()
    {
        delete _paket;
        delete [] _frameBuff;
    }

    bool GLFFmpegCamcorder::setup()
    {
        _codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
        if(_codec == nullptr)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't find the codec for openGL video  recording",
                "GLFFmpegCamcorder")
            );
            return false;
        }

        _context = avcodec_alloc_context3(_codec);
        _context->bit_rate = _bitRate;
        _context->width = _frameWidth;
        _context->height = _frameHeight;
        _context->time_base = (AVRational){1, _frameRate};
        _context->gop_size = 10;
        _context->max_b_frames = 1;
        _context->pix_fmt = PIX_FMT_YUV420P;


        if(avcodec_open2(_context, _codec, nullptr) < 0)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't open codec",
                "GLFFmpegCamcorder")
            );
            return false;
        }


        _srcFrame = avcodec_alloc_frame();
        if(_srcFrame == nullptr)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't allocate frame",
                "GLFFmpegCamcorder")
            );
            return false;
        }

        _srcFrame->format = PIX_FMT_RGB24;
        _srcFrame->width = _context->width;
        _srcFrame->height = _context->height;
        if(av_image_alloc( _srcFrame->data,   _srcFrame->linesize,
                           _context->width,   _context->height,
                           PIX_FMT_RGB24, 32) < 0)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't allocate raw picture buffer",
                "GLFFmpegCamcorder")
            );
            return false;
        }


        _dstFrame = avcodec_alloc_frame();
        if(_dstFrame == nullptr)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't allocate frame",
                "GLFFmpegCamcorder")
            );
            return false;
        }

        _dstFrame->format = _context->pix_fmt;
        _dstFrame->width = _context->width;
        _dstFrame->height = _context->height;
        if(av_image_alloc( _dstFrame->data,   _dstFrame->linesize,
                           _context->width,   _context->height,
                           _context->pix_fmt, 32) < 0)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't allocate raw picture buffer",
                "GLFFmpegCamcorder")
            );
            return false;
        }

        _swsContext = sws_getContext(
                    _frameWidth, _frameHeight, PIX_FMT_RGB24,
                    _frameWidth, _frameHeight, PIX_FMT_YUV420P,
                    SWS_BICUBIC, nullptr, nullptr, nullptr);

        _videoStream.open(_fileName.c_str());
        if(!_videoStream.is_open())
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't open video file",
                "GLFFmpegCamcorder")
            );
            return false;
        }

        _frameBuff = new unsigned char[_frameWidth*_frameHeight*3];

        return true;
    }

    void GLFFmpegCamcorder::recordFrame()
    {
        _dstFrame->pts = _framePts++;
        av_init_packet(_paket);
        _paket->data = nullptr;
        _paket->size = 0;

        glReadPixels(0, 0, _frameWidth, _frameHeight,
                     GL_RGB, GL_UNSIGNED_BYTE, _srcFrame->data[0]);

        for(int j=0; j<_frameHeight/2; ++j)
        {
            for(int i=0; i<_frameWidth; ++i)
            {
                int srcIdx = (j*_frameWidth+i)*3;
                int dstIdx = ((_frameHeight-1-j)*_frameWidth+i)*3;
                swap(_srcFrame->data[0][srcIdx + 0],
                     _srcFrame->data[0][dstIdx + 0]);
                swap(_srcFrame->data[0][srcIdx + 1],
                     _srcFrame->data[0][dstIdx + 1]);
                swap(_srcFrame->data[0][srcIdx + 2],
                     _srcFrame->data[0][dstIdx + 2]);
            }
        }

        sws_scale(_swsContext, _srcFrame->data, _srcFrame->linesize,
                  0, _frameHeight, _dstFrame->data, _dstFrame->linesize);

        int hasNext = 1;
        if(avcodec_encode_video2(_context, _paket, _dstFrame, &hasNext) < 0)
        {
            getLog().postMessage(new Message(
                'E', false,
                "Couldn't encode frame",
                "GLFFmpegCamcorder")
            );
        }
        else
        {
            _videoStream.write((char*)_paket->data, _paket->size);
            av_free_packet(_paket);
        }
    }

    bool GLFFmpegCamcorder::finalise()
    {
        int hasNext = 1; // true
        while(hasNext)
        {
            if(avcodec_encode_video2(_context, _paket, nullptr, &hasNext) < 0)
            {
                getLog().postMessage(new Message(
                    'E', false,
                    "Couldn't encode frame",
                    "GLFFmpegCamcorder")
                );
                return false;
            }
            else
            {
                _videoStream.write((char*)_paket->data, _paket->size);
                av_free_packet(_paket);
            }
        }

        _videoStream << 0x00 << 0x00 << 0x01 << 0xb7;
        _videoStream.close();
        avcodec_close(_context);
        av_free(_context);
        av_freep(&_dstFrame->data[0]);
        av_free(_dstFrame);

        return true;
    }
}

#endif //FFMPEG_ENABLED
