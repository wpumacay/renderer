#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoDecoder.h>

// @todo: complete it : make it double-buffered, re-structure methods, add loop functionality, add async mode, etc.

namespace engine
{
    std::string ToString( const eDecodingMode& mode )
    {
        if ( mode == eDecodingMode::SYNC ) return "synchronous";
        if ( mode == eDecodingMode::ASYNC ) return "asynchronous";

        return "undefined";
    }

    std::string ToString( const eDecodingState& state )
    {
        if ( state == eDecodingState::IDLE ) return "idle";
        if ( state == eDecodingState::READY ) return "ready";
        if ( state == eDecodingState::DECODING ) return "decoding";
        if ( state == eDecodingState::FINISHED ) return "finished";

        return "undefined";
    }

    std::unique_ptr< uint8_t[] > DecodeOneFrame( AVFormatContext* format_ctx,
                                                 AVCodecContext* codec_ctx,
                                                 AVFrame* frame_av,
                                                 AVPacket* packet_av,
                                                 SwsContext* sws_ctx,
                                                 ssize_t video_frame_index )
    {
        const ssize_t frame_width = codec_ctx->width;
        const ssize_t frame_height = codec_ctx->height;
        const ssize_t frame_npixels = frame_width * frame_height;
        const ssize_t frame_nbytes = 3 * frame_npixels;

        auto frame_data = std::unique_ptr< uint8_t[] >( new uint8_t[frame_nbytes] );
        memset( frame_data.get(), 0, sizeof( uint8_t ) * frame_nbytes );

        while ( av_read_frame( format_ctx, packet_av ) >= 0 )
        {
            if ( packet_av->stream_index == video_frame_index )
            {
                ENGINE_CORE_TRACE( "DecodeOneFrame >>> packet-pts: {0}", packet_av->pts );
                ssize_t response = avcodec_send_packet( codec_ctx, packet_av );
                if( response < 0 )
                {
                    ENGINE_CORE_ERROR( "DecodeOneFrame >>> Couldn't send a packet to the decoder: {0}", response );
                    break; // no need to unref, as packet will be freed during out of scope (unique_ptr)
                }

                bool finish_decoding = false;
                while ( response >= 0 )
                {
                    response = avcodec_receive_frame( codec_ctx, frame_av );
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
                        ENGINE_CORE_TRACE( "\tframe-type        : {0}", av_get_picture_type_char( frame_av->pict_type ) );
                        ENGINE_CORE_TRACE( "\tframe-size(bytes) : {0}", frame_av->pkt_size );
                        ENGINE_CORE_TRACE( "\tframe-pts         : {0}", frame_av->pts );
                        ENGINE_CORE_TRACE( "\tframe-keyframe    : {0}", frame_av->key_frame );
                        ENGINE_CORE_TRACE( "\tframe-pic-num(DTS): {0}", frame_av->coded_picture_number );

                        uint8_t* dest_buffers[4] = { frame_data.get(), NULL, NULL, NULL };
                        int dest_linesizes[4] = { (int) frame_width * 3, 0, 0, 0 };
                        // @todo: iterate only over valid channels
                        // @source: https://lists.ffmpeg.org/pipermail/ffmpeg-user/2011-May/000976.html
                        frame_av->data[0] += frame_av->linesize[0] * ( frame_height - 1 );
                        frame_av->data[1] += frame_av->linesize[1] * ( frame_height - 1 );
                        frame_av->data[2] += frame_av->linesize[2] * ( frame_height - 1 );
                        frame_av->linesize[0] = -frame_av->linesize[0];
                        frame_av->linesize[1] = -frame_av->linesize[1];
                        frame_av->linesize[2] = -frame_av->linesize[2];
                        sws_scale( sws_ctx,
                                   frame_av->data,
                                   frame_av->linesize,
                                   0,
                                   frame_av->height,
                                   dest_buffers,
                                   dest_linesizes );

                        finish_decoding = true;
                        break;
                    }
                }

                if ( finish_decoding )
                    break;
            }
            // Detach the current packet from its buffer (to allow freeing it)
            av_packet_unref( packet_av );
        }

        return std::move( frame_data );
    }

    CVideoDecoder::CVideoDecoder( const eDecodingMode& mode, const std::string& filepath )
        : m_mode( mode ), m_state( eDecodingState::IDLE ), m_loop( false ), m_filepath( filepath )
    {
        m_formatContext = nullptr;
        m_codecContext = nullptr;
        m_lastFrameData = nullptr;

        if ( !g_InitializedFFMPEG )
        {
            av_register_all();
            g_InitializedFFMPEG = true;
        }

        m_frame_av = std::unique_ptr<AVFrame, AVFrameDeleter>( av_frame_alloc() );
        m_packet_av = std::unique_ptr<AVPacket, AVPacketDeleter>( av_packet_alloc() );

        if ( filepath != "" )
            Reset( filepath );
    }

    CVideoDecoder::~CVideoDecoder()
    {
        m_frame_av = nullptr;
        m_packet_av = nullptr;
        m_swsContext = nullptr;
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

        m_swsContext = std::unique_ptr<SwsContext, SwsContextDeleter>( sws_getContext( m_codecContext->width,
                                                                                       m_codecContext->height,
                                                                                       m_codecContext->pix_fmt,
                                                                                       m_codecContext->width,
                                                                                       m_codecContext->height,
                                                                                       AV_PIX_FMT_RGB24,
                                                                                       SWS_BILINEAR, NULL, NULL, NULL ) );

        m_state = eDecodingState::READY;
    }

    void CVideoDecoder::BeginDecoding()
    {
        if ( m_state != eDecodingState::READY )
        {
            ENGINE_CORE_WARN( "CVideoDecoder::BeginDecoding >>> video decoder hasn't been properly setup. \
                               Use Reset(movie_file_path) to configure it properly before decoding." );
            return;
        }

        if ( m_mode == eDecodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoDecoder::BeginDecoding >>> started video-decoding thread" );
            // @todo: Start working thread for decoding
            // _StartVideoDecodingThread();
        }
    }

    void CVideoDecoder::EndDecoding()
    {
        if ( m_mode == eDecodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoDecoder::EndDecoding >>> stopped video-decoding thread" );
            // @todo: check where to call join to stop working thread (separate update-function? or when calling next-frame?)
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

        // Define some properties of the frame
        const ssize_t frame_width = m_codecContext->width;
        const ssize_t frame_height = m_codecContext->height;
        const ssize_t frame_npixels = frame_width * frame_height;
        const ssize_t frame_nbytes = 3 * frame_npixels;

        if ( m_state == eDecodingState::IDLE )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::GetNextFrame >>> decoder is not ready|hasn't been setup. \
                                Use Reset(movie_file_path) to configure it properly before decoding." );
            return nullptr;
        }

        if ( m_state == eDecodingState::DECODING || m_state == eDecodingState::FINISHED )
        {
            // We're here if in async-mode and the decoding-thread is still working, or if in either
            // mode we've finished decoding all frames (and no loop), so return the last valid frame
            auto last_frame_data_cp = std::unique_ptr<uint8_t[]>( new uint8_t[frame_nbytes] );
            if ( m_lastFrameData ) // there's a last-frame, so copy its data
                memcpy( last_frame_data_cp.get(), m_lastFrameData.get(), sizeof( uint8_t ) * frame_nbytes );
            else // no last frame data, so just return a black image
                memset( last_frame_data_cp.get(), 0, sizeof( uint8_t ) * frame_nbytes );
            return std::move( last_frame_data_cp );
        }

        if ( m_state == eDecodingState::READY )
        {
            if ( m_mode == eDecodingMode::SYNC )
            {
                auto next_frame_data = DecodeOneFrame( m_formatContext.get(),
                                                       m_codecContext.get(),
                                                       m_frame_av.get(),
                                                       m_packet_av.get(),
                                                       m_swsContext.get(),
                                                       m_videoStreamIndex );
                m_lastFrameData = std::unique_ptr< uint8_t[] >( new uint8_t[frame_nbytes] );
                memcpy( m_lastFrameData.get(), next_frame_data.get(), sizeof( uint8_t ) * frame_nbytes );

                return std::move( next_frame_data );
            }
            else
            {
                // @todo: grab mutex and copy last decoded frame
            }
        }

        ENGINE_CORE_WARN( "CVideoDecoder::GetNextFrame >>> current mode {0} not supported in state {1}",
                           ToString( m_mode ), ToString( m_state ) );
        return nullptr;
    }

    std::unique_ptr<uint8_t[]> CVideoDecoder::GetLastFrame()
    {
        const ssize_t frame_width = m_codecContext->width;
        const ssize_t frame_height = m_codecContext->height;
        const ssize_t frame_npixels = frame_width * frame_height;
        const ssize_t frame_nbytes = 3 * frame_npixels;

        auto last_frame_data = std::unique_ptr< uint8_t[] >( new uint8_t[frame_nbytes] );
        if ( m_lastFrameData ) // there's a last-frame, so copy its data
            memcpy( last_frame_data.get(), m_lastFrameData.get(), sizeof( uint8_t ) * frame_nbytes );
        else // no last frame data, so just return a black image
            memset( last_frame_data.get(), 0, sizeof( uint8_t ) * frame_nbytes );

        return std::move( last_frame_data );
    }

    ssize_t CVideoDecoder::GetCurrentFrameWidth() const
    {
        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::GetCurrentFrameWidth >>> there's no valid codec_ctx" );
            return -1;
        }

        return m_codecContext->width;
    }

    ssize_t CVideoDecoder::GetCurrentFrameHeight() const
    {
        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoDecoder::GetCurrentFrameHeight >>> there's no valid codec_ctx" );
            return -1;
        }

        return m_codecContext->height;
    }
}

#endif /* ENGINE_USE_FFMPEG */