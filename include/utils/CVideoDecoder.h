#pragma once

#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoCommon.h>

namespace engine
{
    enum class eDecodingMode : uint8_t
    {
        SYNC = 0,
        ASYNC
    };

    std::unique_ptr<uint8_t[]> DecodeOneFrame( AVCodecContext* codec_ctx );

    class CVideoDecoder
    {
    public :

        CVideoDecoder( const eDecodingMode& mode, const std::string& filepath = "" );
        CVideoDecoder( const CVideoDecoder& other ) = delete;
        CVideoDecoder& operator=( const CVideoDecoder& other ) = delete;
        ~CVideoDecoder();

        void Reset( const std::string& video_filepath );
        void BeginDecoding();
        void EndDecoding();

        std::unique_ptr<uint8_t[]> GetNextFrame();

        bool IsDecoding() const { return m_decoding; }

        bool loop() const { return m_loop; }

        eDecodingMode mode() const { return m_mode; }

        std::string filepath() const { return m_filepath; }

    private :

        std::unique_ptr<AVCodecContext, AVCodecContextDeleter> m_codecContext;

        std::unique_ptr<AVFormatContext, AVFormatContextDeleter> m_formatContext;

        std::unique_ptr<uint8_t[]> m_lastFrameData;

        bool m_decoding;

        bool m_loop;

        eDecodingMode m_mode;

        std::string m_filepath;

        static bool s_InitializedFFMPEG;
    };

}

#endif /* ENGINE_USE_FFMPEG */