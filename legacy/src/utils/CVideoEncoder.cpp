#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoEncoder.h>

// @todo: complete it : make it double-buffered, re-structure methods, add loop functionality, add async mode, etc.

namespace engine
{

    std::string ToString( const eEncodingMode& mode )
    {
        if ( mode == eEncodingMode::SYNC ) return "synchronous";
        if ( mode == eEncodingMode::ASYNC ) return "asynchronous";

        return "undefined";
    }

    std::string ToString( const eEncodingState& state )
    {
        if ( state == eEncodingState::IDLE ) return "idle";
        if ( state == eEncodingState::READY ) return "ready";
        if ( state == eEncodingState::ENCODING ) return "encoding";

        return "undefined";
    }

    void EncodeOneFrame( AVFormatContext* format_ctx,
                         AVCodecContext* codec_ctx,
                         AVStream* stream,
                         AVFrame* frame_av,
                         AVPacket* packet_av,
                         SwsContext* sws_ctx,
                         const uint8_t* frame_data,
                         size_t frame_index )
    {
        const ssize_t frame_width = codec_ctx->width;
        const ssize_t frame_height = codec_ctx->height;
        const ssize_t frame_npixels = frame_width * frame_height;
        const ssize_t frame_nbytes = 3 * frame_npixels;

        // Convert rgb-frame-data into av-frame
        const uint8_t* src_buffers[4] = { frame_data, NULL, NULL, NULL };
        const int src_linesizes[4] = { (int) frame_width * 3, 0, 0, 0 };
        sws_scale( sws_ctx,
                   src_buffers,
                   src_linesizes,
                   0,
                   frame_height,
                   frame_av->data,
                   frame_av->linesize );

        ssize_t response = avcodec_send_frame( codec_ctx, frame_av );
        while ( response >= 0 )
        {
            response = avcodec_receive_packet( codec_ctx, packet_av );
            if ( response == AVERROR(EAGAIN) || response == AVERROR_EOF )
            {
                break;
            }
            else if ( response < 0 )
            {
                ENGINE_CORE_ERROR( "EncodeOneFrame >>> couldn't send a packet to the encoder for encoding: {0}", response );
                return; // @todo: no need to unref packet?
            }

            packet_av->pts = frame_index;
            packet_av->stream_index = 0; // first stream is video stream
            packet_av->duration = stream->time_base.den / stream->time_base.num / stream->avg_frame_rate.num * stream->avg_frame_rate.den;
            av_packet_rescale_ts( packet_av, codec_ctx->time_base, stream->time_base );
            response = av_interleaved_write_frame( format_ctx, packet_av );
            if ( response != 0 )
            {
                ENGINE_CORE_ERROR( "EncodeOneFrame >>> couldn't write packet to the container. Error-code: {0}", response );
                break; // @todo: no need to unref packet?
            }
        }
        av_packet_unref( packet_av );
    }

    CVideoEncoder::CVideoEncoder( const eEncodingMode& mode,
                                  const std::string& filepath,
                                  ssize_t width,
                                  ssize_t height )
        : m_mode( mode ), m_state( eEncodingState::IDLE ), m_filepath( filepath ), m_width( width ), m_height( height )
    {
        m_formatContext = nullptr;
        m_codecContext = nullptr;
        m_streamRef = nullptr;

        if ( !g_InitializedFFMPEG )
        {
            av_register_all();
            g_InitializedFFMPEG = true;
        }

        m_frame_av = std::unique_ptr<AVFrame, AVFrameDeleter>( av_frame_alloc() );
        m_packet_av = std::unique_ptr<AVPacket, AVPacketDeleter>( av_packet_alloc() );

        Reset( filepath );
    }

    CVideoEncoder::~CVideoEncoder()
    {
        m_frame_av = nullptr;
        m_packet_av = nullptr;
        m_swsContext = nullptr;
        m_formatContext = nullptr;
        m_codecContext = nullptr;
    }

    void CVideoEncoder::Reset( const std::string& filepath )
    {
        m_filepath = filepath;
        m_frame_index = 0;

        AVFormatContext* ptr_formatContext = nullptr;
        if ( avformat_alloc_output_context2( &ptr_formatContext, NULL, NULL, filepath.c_str() ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't allocate an output format-context" );
            return;
        }

        // Create a new stream in the output format-context, and grab a reference
        m_streamRef = avformat_new_stream( ptr_formatContext, NULL );

        const char* enc_video_codec_name = "libx265";
        const char* enc_video_codec_priv_key = "x265-params";
        const char* enc_video_codec_priv_value = "keyint=60:min-keyint=60:scenecut=0";
        const int64_t enc_video_codec_bitrate = 2 * 1000 * 1000; // @todo: expose to the user?
        const int32_t enc_video_codec_rc_buffer_size = 4 * 1000 * 1000; // @todo: expose to the user?
        const int64_t enc_video_codec_rc_max_rate = 2 * 1000 * 1000; // @todo: expose to the user?
        const int64_t enc_video_codec_rc_min_rate = 2.5 * 1000 * 1000; // @todo: expose to the user?
        const int32_t enc_video_codec_frame_rate = 60; // @todo: expose to the user?

        // Find the codec related to our codec-configuration
        const AVCodec* ptr_video_codec = avcodec_find_encoder_by_name( enc_video_codec_name );
        if ( !ptr_video_codec )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't find required encoder with name {0}", enc_video_codec_name );
            return;
        }
        // Create and configure the codec-context
        m_codecContext = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>( avcodec_alloc_context3( ptr_video_codec ) );
        av_opt_set( m_codecContext->priv_data, "preset", "fast", 0 ); // @todo: using config. from source, tweak as required
        av_opt_set( m_codecContext->priv_data, enc_video_codec_priv_key, enc_video_codec_priv_value, 0 ); // @todo: using config. from source, tweak as required
        m_codecContext->width = m_width;
        m_codecContext->height = m_height;
        //// m_codecContext->sample_aspect_ratio = 0; // @todo: check if necessary
        m_codecContext->pix_fmt = AV_PIX_FMT_YUV444P;
        m_codecContext->time_base = (AVRational){ 1, enc_video_codec_frame_rate };
        m_codecContext->framerate = (AVRational){ enc_video_codec_frame_rate, 1 };
        m_codecContext->bit_rate = enc_video_codec_bitrate;
        m_codecContext->rc_buffer_size = enc_video_codec_rc_buffer_size;
        m_codecContext->rc_max_rate = enc_video_codec_rc_max_rate;
        m_codecContext->rc_min_rate = enc_video_codec_rc_min_rate;
        m_streamRef->time_base = m_codecContext->time_base;
        m_streamRef->avg_frame_rate = m_codecContext->framerate;

        if ( avcodec_open2( m_codecContext.get(), ptr_video_codec, NULL ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't open required codec" );
            return;
        }

        // Copy parameters from the codec-context to the stream
        avcodec_parameters_from_context( m_streamRef->codecpar, m_codecContext.get() );

        // Allocate space for the AVFrame
        m_frame_av->format = AV_PIX_FMT_YUV444P;
        m_frame_av->width = m_width;
        m_frame_av->height = m_height;
        m_frame_av->pict_type = AV_PICTURE_TYPE_NONE;
        if ( av_frame_get_buffer( m_frame_av.get(), 0 ) )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't allocate video frame" );
            return;
        }
        // Ensure frame is writable
        if ( av_frame_make_writable( m_frame_av.get() ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't make frame writable" );
            return;
        }

        // Open the output file
        if ( avio_open( &ptr_formatContext->pb, filepath.c_str(), AVIO_FLAG_WRITE ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> couldn't open output file: {0}", filepath );
            return;
        }
        if ( avformat_write_header( ptr_formatContext, NULL ) < 0 )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::Reset >>> an error occurred while opening the output file: {0}", filepath );
            return;
        }

        // Keep ownership of the format-context
        m_formatContext = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>( ptr_formatContext );

        // Software-scale context (to handle conversion from RGB888 to YUVxyz(from codec))
        m_swsContext = std::unique_ptr<SwsContext, SwsContextDeleter>( sws_getContext( m_codecContext->width,
                                                                                       m_codecContext->height,
                                                                                       AV_PIX_FMT_RGB24,
                                                                                       m_codecContext->width,
                                                                                       m_codecContext->height,
                                                                                       m_codecContext->pix_fmt,
                                                                                       SWS_BILINEAR, NULL, NULL, NULL ) );

        ENGINE_CORE_TRACE( "CVideoEncoder::Reset >>> video-format information" );
        ENGINE_CORE_TRACE( "\tvideo-format  : {0}", m_formatContext->oformat->long_name );
        ENGINE_CORE_TRACE( "\tvideo-nstreams: {0}", m_formatContext->nb_streams );
        ENGINE_CORE_TRACE( "CVideoEncoder::Reset >>> video-codec information" );
        ENGINE_CORE_TRACE( "\tvideo-codec-resolution-width  : {0}", m_streamRef->codecpar->width );
        ENGINE_CORE_TRACE( "\tvideo-codec-resolution-height : {0}", m_streamRef->codecpar->height );
        ENGINE_CORE_TRACE( "\tvideo-codec-stream-index      : {0}", m_streamRef->index );
        ENGINE_CORE_TRACE( "\tvideo-codec-stream-framerate  : {0}/{1}", m_streamRef->avg_frame_rate.num, m_streamRef->avg_frame_rate.den );
        ENGINE_CORE_TRACE( "\tvideo-codec-name              : {0}", m_codecContext->codec->long_name );
        ENGINE_CORE_TRACE( "\tvideo-codec-id                : {0}", m_codecContext->codec->id );
        ENGINE_CORE_TRACE( "\tvideo-codec-bitrate           : {0}", m_streamRef->codecpar->bit_rate );

        m_state = eEncodingState::READY;
    }

    void CVideoEncoder::BeginEncoding()
    {
        if ( m_state != eEncodingState::READY )
        {
            ENGINE_CORE_WARN( "CVideoEncoder::BeginEncoding >>> video encoder hasn't been properly setup. \
                               Use Reset(movie_file_path) to configure it properly before encoding." );
            return;
        }

        if ( m_mode == eEncodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoEncoder::BeginEncoding >>> started video-encoding thread" );
            // @todo: Start working thread for encoding
            // _StartVideoEncodingThread();
        }
    }

    void CVideoEncoder::EndEncoding()
    {
        if ( m_mode == eEncodingMode::ASYNC )
        {
            ENGINE_CORE_TRACE( "CVideoEncoder::EndEncoding >>> stopped video-encoding thread" );
            // @todo: check where to call join to stop working thread (separate update-function? or when calling next-frame?)
            // _StopVideoEncodingThread();
        }

        // Flush video encoder and write stream trailer to output file
        avcodec_send_frame( m_codecContext.get(), NULL );
        av_write_trailer( m_formatContext.get() );
        m_state = eEncodingState::IDLE;
    }

    // sync: encode frame data directly in same thread (no extra copy required)
    // async: copy data into cache to encode in thread (perhaps many frames come quickly)
    void CVideoEncoder::SubmitFrame( const uint8_t* frame_data, ssize_t frame_width, ssize_t frame_height )
    {
        if ( !m_formatContext )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::SubmitFrame >>> encoding requires format-context" );
            return;
        }

        if ( !m_codecContext )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::SubmitFrame >>> encoding requires codec-context" );
            return;
        }

        if ( ( frame_width != m_width ) || ( frame_height != m_height ) )
        {
            ENGINE_CORE_ERROR( "CVideoEncoder::SubmitFrame >>> dimensions mismatch: given=({0},{1}), \
                                expected=({2},{3})", frame_width, frame_height, m_width, m_height );
            return;
        }

        if ( m_state == eEncodingState::IDLE )
        {
            ENGINE_CORE_WARN( "CVideoEncoder::SubmitFrame >>> encoder is either not setup or it's already \
                               finished encoding. Use Reset(movie_file_path) to re-configure it." );
            return;
        }

        if ( m_mode == eEncodingMode::SYNC )
        {
            EncodeOneFrame( m_formatContext.get(),
                            m_codecContext.get(),
                            m_streamRef,
                            m_frame_av.get(),
                            m_packet_av.get(),
                            m_swsContext.get(),
                            frame_data,
                            m_frame_index++ );
            return;
        }
        else if ( m_mode == eEncodingMode::ASYNC )
        {
            // @todo: add functionality for asynchronous mode
        }

        ENGINE_CORE_WARN( "CVideoEncoder::SubmitFrame >>> current mode {0} not supported in state {1}",
                           ToString( m_mode ), ToString( m_state ) );
    }

}

#endif /* ENGINE_USE_FFMPEG */
