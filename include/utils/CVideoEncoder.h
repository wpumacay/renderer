#pragma once

#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoCommon.h>

namespace engine
{
    enum class eEncodingMode : uint8_t
    {
        SYNC = 0,
        ASYNC
    };

    std::string ToString( const eEncodingMode& mode );

    enum class eEncodingState : uint8_t
    {
        IDLE = 0,
        READY,
        ENCODING
    };

    std::string ToString( const eEncodingState& state );

    void EncodeOneFrame( AVFormatContext* format_ctx,
                         AVCodecContext* codec_ctx,
                         AVStream* stream,
                         AVFrame* frame_av,
                         AVPacket* packet_av,
                         const uint8_t* frame_data );

    class CVideoEncoder
    {
    public :

        CVideoEncoder( const eEncodingMode& mode,
                       const std::string& filepath,
                       ssize_t width,
                       ssize_t height );
        CVideoEncoder( const CVideoEncoder& other ) = delete;
        CVideoEncoder& operator=( const CVideoEncoder& other ) = delete;
        ~CVideoEncoder();

        void Reset( const std::string& filepath );
        void BeginEncoding();
        void EndEncoding();

        void SubmitFrame( const uint8_t* frame_data, ssize_t frame_width, ssize_t frame_height );

        eEncodingMode mode() const { return m_mode; }

        eEncodingState state() const { return m_state; }

        std::string filepath() const { return m_filepath; }

        ssize_t width() const { return m_width; }

        ssize_t height() const { return m_height; }

    private :

        std::unique_ptr<AVFormatContext, AVFormatContextDeleter> m_formatContext;

        std::unique_ptr<AVCodecContext, AVCodecContextDeleter> m_codecContext;

        std::unique_ptr<AVFrame, AVFrameDeleter> m_frame_av;

        std::unique_ptr<AVPacket, AVPacketDeleter> m_packet_av;

        AVStream* m_streamRef;

        eEncodingMode m_mode;

        eEncodingState m_state;

        std::string m_filepath;

        ssize_t m_width;

        ssize_t m_height;
    };

}

#endif /* ENGINE_USE_FFMPEG */