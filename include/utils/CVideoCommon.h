#pragma once

#ifdef ENGINE_USE_FFMPEG

#include <CCommon.h>
#include <utils/CLogger.h>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/opt.h>
}

namespace engine
{
    extern bool g_InitializedFFMPEG;

    struct AVCodecContextDeleter
    {
        void operator()( AVCodecContext* codec_context ) const;
    };

    struct AVFormatContextDeleter
    {
        void operator()( AVFormatContext* format_context ) const;
    };

    struct AVPacketDeleter
    {
        void operator()( AVPacket* packet ) const;
    };

    struct AVFrameDeleter
    {
        void operator()( AVFrame* frame ) const;
    };

    struct SwsContextDeleter
    {
        void operator()( SwsContext* sws_context ) const;
    };
}

#endif /* ENGINE_USE_FFMPEG */