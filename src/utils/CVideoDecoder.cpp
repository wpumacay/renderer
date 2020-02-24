#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoDecoder.h>

namespace engine
{
    std::unique_ptr< uint8_t[] > DecodeOneFrame( AVCodecContext* codec_ctx )
    {
        const size_t frame_width = codec_ctx->width;
        const size_t frame_height = codec_ctx->height;
        const size_t frame_npixels = frame_width * frame_height;
        const size_t frame_nbytes = 3 * frame_npixels;

        auto frame_data = std::unique_ptr< uint8_t[] >( new uint8_t[frame_nbytes] );
        memset( (void*)frame_data.get(), 0, sizeof( uint8_t ) * frame_nbytes );

        auto libav_frame = std::unique_ptr<AVFrame>( av_frame_alloc() );
        auto libav_packet = std::unique_ptr<AVPacket>( av_packet_alloc() );
        if ( !libav_frame || !libav_packet )
        {
            ENGINE_CORE_ERROR( "DecodeOneFrame >>> Couldn't allocate avframe nor avpacket" );
            return std::move( frame_data );
        }

////         while ( av_read_frame( av_format_ctx, av_packet ) >= 0 )
////         {
//// 
////         }

        return std::move( frame_data );
    }

    bool CVideoDecoder::s_InitializedFFMPEG = false;

    CVideoDecoder::CVideoDecoder( const eDecodingMode& mode, const std::string& filepath )
        : m_mode( mode ), m_decoding( false ), m_loop( false ), m_filepath( filepath )
    {
        m_formatContext = nullptr;
        m_codecContext = nullptr;
        m_lastFrameData = nullptr;

        if ( !CVideoDecoder::s_InitializedFFMPEG )
        {
            av_register_all();
            CVideoDecoder::s_InitializedFFMPEG = true;
        }

        if ( filepath != "" )
            Reset( filepath );
    }

    CVideoDecoder::~CVideoDecoder()
    {
        m_formatContext = nullptr;
        m_codecContext = nullptr;
    }

    void CVideoDecoder::Reset( const std::string& filepath )
    {
        m_filepath = filepath;

        auto ptr_formatContext = avformat_alloc_context();
        if ( !ptr_formatContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't allocate format-context" );
            return;
        }

        if ( avformat_open_input( &ptr_formatContext, filepath.c_str(), NULL, NULL ) != 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't open video-file: {0}", filepath );
            return;
        }

        m_formatContext = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>( ptr_formatContext );

        ENGINE_CORE_TRACE( "CVideoDecoder::Reset >>> video information" );
        ENGINE_CORE_TRACE( "\tvideo-format  : {0}", m_formatContext->iformat->long_name );
        ENGINE_CORE_TRACE( "\tvideo-duration: {1}", m_formatContext->duration );
        ENGINE_CORE_TRACE( "\tvideo-nstreams: {2}", m_formatContext->nb_streams );

        const AVCodec* video_codec = nullptr;
        const AVCodecParameters* video_codec_parameters = nullptr;
        for ( size_t i = 0; i < m_formatContext->nb_streams; i++ )
        {
            if ( auto stream = m_formatContext->streams[i] )
            {
                // Check if the codec required in the video-file is supported
                if ( auto stream_codec = avcodec_find_decoder( stream->codecpar->codec_id ) )
                {
                    // Check if this stream is a video-stream
                    if ( stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO )
                    {
                        video_codec = stream_codec;
                        video_codec_parameters = stream->codecpar;
                        break;
                    }
                }
            }
        }

        if ( !video_codec || !video_codec_parameters )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't find a video-stream in {0}", filepath );
            return;
        }

        m_codecContext = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>( avcodec_alloc_context3( video_codec ) );
        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't allocate codec-context" );
            return;
        }

        if ( avcodec_parameters_to_context( m_codecContext.get(), video_codec_parameters ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't initialize code-context" );
            return;
        }

        if ( avcodec_open2( m_codecContext.get(), video_codec, NULL ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::DecodeFile >>> Couldn't open codec" );
            return;
        }
    }

    void CVideoDecoder::BeginDecoding()
    {

    }

    void CVideoDecoder::EndDecoding()
    {

    }

    std::unique_ptr<uint8_t[]> CVideoDecoder::GetNextFrame()
    {
        return nullptr;
    }
}

#endif /* ENGINE_USE_FFMPEG */