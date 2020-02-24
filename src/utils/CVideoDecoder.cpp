#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoDecoder.h>

namespace engine
{
    std::string ToString( const eDecodingMode& mode )
    {
        if ( mode == eDecodingMode::SYNC ) return "synchronous";
        else if ( mode == eDecodingMode::ASYNC ) return "asynchronous";

        return "undefined";
    }

    std::string ToString( const eDecodingState& state )
    {
        if ( state == eDecodingState::IDLE ) return "idle";
        else if ( state == eDecodingState::READY ) return "ready";
        else if ( state == eDecodingState::DECODING ) return "decoding";
        else if ( state == eDecodingState::FINISHED ) return "finished";

        return "undefined";
    }

    std::unique_ptr< uint8_t[] > DecodeOneFrame( AVFormatContext* format_ctx,
                                                 AVCodecContext* codec_ctx,
                                                 ssize_t video_frame_index )
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

        while ( av_read_frame( format_ctx, libav_packet.get() ) >= 0 )
        {
            if ( libav_packet->stream_index == video_frame_index )
            {
                ENGINE_CORE_TRACE( "DecodeOneFrame >>> packet-pts: {0}", libav_packet->pts );
                ssize_t response = avcodec_send_packet( codec_ctx, libav_packet.get() );
                if( response < 0 )
                {
                    ENGINE_CORE_ERROR( "DecodeOneFrame >>> Couldn't send a packet to the decoder: {0}", response );
                    break; // no need to unref, as packet will be freed during out of scope (unique_ptr)
                }

                bool finish_decoding = false;
                while ( response >= 0 )
                {
                    response = avcodec_receive_frame( codec_ctx, libav_frame.get() );
                    if ( response == AVERROR(EAGAIN) || response == AVERROR_EOF )
                    {
                        break;
                    }
                    else if ( response < 0 )
                    {
                        ENGINE_CORE_ERROR( "DecodeOneFrame >>> couldn't receive a frame from the decoder: {0}", response );
                        finish_decoding = true;
                        break;
                    }

                    if ( response >= 0 )
                    {
                        ENGINE_CORE_TRACE( "DecodeOneFrame >>> frame-information" );
                        ENGINE_CORE_TRACE( "\tframe-number      : {0}", codec_ctx->frame_number );
                        ENGINE_CORE_TRACE( "\tframe-type        : {0}", av_get_picture_type_char( libav_frame->pict_type ) );
                        ENGINE_CORE_TRACE( "\tframe-size(bytes) : {0}", libav_frame->pkt_size );
                        ENGINE_CORE_TRACE( "\tframe-pts         : {0}", libav_frame->pts );
                        ENGINE_CORE_TRACE( "\tframe-keyframe    : {0}", libav_frame->key_frame );
                        ENGINE_CORE_TRACE( "\tframe-pic-num(DTS): {0}", libav_frame->coded_picture_number );
                        finish_decoding = true;
                        break;
                    }
                }

                if ( finish_decoding )
                    break;
            }
            // Detach the current packet from its buffer (to allow freeing it)
            av_packet_unref( libav_packet.get() );
        }

        return std::move( frame_data );
    }

    bool CVideoDecoder::s_InitializedFFMPEG = false;

    CVideoDecoder::CVideoDecoder( const eDecodingMode& mode, const std::string& filepath )
        : m_mode( mode ), m_state( eDecodingState::IDLE ), m_loop( false ), m_filepath( filepath )
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
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't allocate format-context" );
            return;
        }

        if ( avformat_open_input( &ptr_formatContext, filepath.c_str(), NULL, NULL ) != 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't open video-file: {0}", filepath );
            return;
        }

        m_formatContext = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>( ptr_formatContext );

        ENGINE_CORE_TRACE( "CVideoDecoder::Reset >>> video-format information" );
        ENGINE_CORE_TRACE( "\tvideo-format  : {0}", m_formatContext->iformat->long_name );
        ENGINE_CORE_TRACE( "\tvideo-duration: {0}", m_formatContext->duration );
        ENGINE_CORE_TRACE( "\tvideo-nstreams: {0}", m_formatContext->nb_streams );

        if ( avformat_find_stream_info( m_formatContext.get(), NULL ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't find stream information for file {0}", filepath );
            return;
        }

        ssize_t video_stream_index = -1;
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
                        video_stream_index = i;
                        ENGINE_CORE_TRACE( "CVideoDecoder::Reset >>> video-codec information" );
                        ENGINE_CORE_TRACE( "\tvideo-codec-resolution-width  : {0}", video_codec_parameters->width );
                        ENGINE_CORE_TRACE( "\tvideo-codec-resolution-height : {0}", video_codec_parameters->height );
                        ENGINE_CORE_TRACE( "\tvideo-codec-stream-index      : {0}", video_stream_index );
                        ENGINE_CORE_TRACE( "\tvideo-codec-name              : {0}", video_codec->long_name );
                        ENGINE_CORE_TRACE( "\tvideo-codec-id                : {0}", video_codec->id );
                        ENGINE_CORE_TRACE( "\tvideo-codec-bitrate           : {0}", video_codec_parameters->bit_rate );
                        break;
                    }
                }
            }
        }

        if ( !video_codec || !video_codec_parameters || video_stream_index == -1 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't find a video-stream in {0}", filepath );
            return;
        }

        m_videoStreamIndex = video_stream_index;
        m_codecContext = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>( avcodec_alloc_context3( video_codec ) );
        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't allocate codec-context" );
            return;
        }

        if ( avcodec_parameters_to_context( m_codecContext.get(), video_codec_parameters ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't initialize code-context" );
            return;
        }

        if ( avcodec_open2( m_codecContext.get(), video_codec, NULL ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::Reset >>> Couldn't open codec" );
            return;
        }

        m_state = eDecodingState::READY;
    }

    void CVideoDecoder::BeginDecoding()
    {
        if ( m_state != eDecodingState::READY )
            return;

        if ( m_mode == eDecodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoDecoder::BeginDecoding >>> started video-decoding thread" );
            // _StartVideoDecodingThread();
        }
    }

    void CVideoDecoder::EndDecoding()
    {
        if ( m_mode == eDecodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoDecoder::EndDecoding >>> stopped video-decoding thread" );
            // _StopVideoDecodingThread();
        }

        m_state = eDecodingState::IDLE;
    }

    std::unique_ptr<uint8_t[]> CVideoDecoder::GetNextFrame()
    {
        if ( !m_formatContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::GetNextFrame >>> decoding requires format-context" );
            return nullptr;
        }
        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::GetNextFrame >>> decoding requires codec-context" );
            return nullptr;
        }

        if ( m_mode == eDecodingMode::SYNC )
            return std::move( DecodeOneFrame( m_formatContext.get(), m_codecContext.get(), m_videoStreamIndex ) );

        return nullptr;
    }
}

#endif /* ENGINE_USE_FFMPEG */