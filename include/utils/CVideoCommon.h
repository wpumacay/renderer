#pragma once

#ifdef ENGINE_USE_FFMPEG

#include <CCommon.h>
#include <utils/CLogger.h>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

namespace engine
{
    struct AVCodecContextDeleter
    {
        void operator()( AVCodecContext* codec_context ) const
        {
            avcodec_free_context( &codec_context );
        }
    };

    struct AVFormatContextDeleter
    {
        void operator()( AVFormatContext* format_context ) const
        {
            avformat_close_input( &format_context );
        }
    };

    struct AVPacketDeleter
    {
        void operator()( AVPacket* packet ) const
        {
            av_packet_free( &packet );
        }
    };

    struct AVFrameDeleter
    {
        void operator()( AVFrame* frame ) const
        {
            av_frame_free( &frame );
        }
    };
}

#endif /* ENGINE_USE_FFMPEG */