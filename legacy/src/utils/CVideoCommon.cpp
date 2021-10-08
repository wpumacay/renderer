
#include <utils/CVideoCommon.h>

#ifdef ENGINE_USE_FFMPEG

namespace engine
{
    bool g_InitializedFFMPEG = false;

    void AVCodecContextDeleter::operator()( AVCodecContext* codec_context ) const
    {
        avcodec_free_context( &codec_context );
    }

    void AVFormatContextDeleter::operator()( AVFormatContext* format_context ) const
    {
        avformat_close_input( &format_context );
    }

    void AVPacketDeleter::operator()( AVPacket* packet ) const
    {
        av_packet_free( &packet );
    }

    void AVFrameDeleter::operator()( AVFrame* frame ) const
    {
        av_frame_free( &frame );
    }

    void SwsContextDeleter::operator()( SwsContext* sws_context ) const
    {
        sws_freeContext( sws_context );
    }
}

#endif /* ENGINE_USE_FFMPEG */
