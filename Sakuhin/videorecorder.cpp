#include "videorecorder.h"

VideoRecorder::VideoRecorder() {

}

void VideoRecorder::initFormat(const char* filename, int framerate) {
    avformat_alloc_output_context2(&formatContext, NULL, NULL, filename);
    if (!formatContext) {
        exit(1);
    }

    format = formatContext->oformat;

    stream = avformat_new_stream(formatContext, NULL);
    if (!stream) {
        qDebug() << "Could not allocate stream";
        exit(1);
    }

    stream->time_base = (AVRational){1, framerate};
}

void VideoRecorder::initCodec(int width, int height) {
    codec = avcodec_find_encoder(format->video_codec);
    if (!(codec)) {
        qDebug() << "Could not find encoder for " << avcodec_get_name(format->video_codec);
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        qDebug() << "Could not allocate video codec context";
        exit(1);
    }

    codecContext->width = width;
    codecContext->height = height;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    codecContext->time_base = stream->time_base;
    codecContext->bit_rate = 20000000;
    codecContext->gop_size = 5;

    // Some formats want stream headers to be separate.
    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER) {
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        qDebug() << "Could not open video codec: " << ret;
        exit(1);
    }

    ret = avcodec_parameters_from_context(stream->codecpar, codecContext);
    if (ret < 0) {
        qDebug() << "Could not copy the stream parameters";
        exit(1);
    }

    swsContext = sws_getCachedContext(NULL,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGBA,
        codecContext->width, codecContext->height, AV_PIX_FMT_YUV420P,
        0, NULL, NULL, NULL);
}

void VideoRecorder::initFrame() {
    // Packets holds encoded video frames that are ready for writing
    packet = av_packet_alloc();
    if (!packet) {
        qDebug() << "Could not allocate packet";
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        qDebug() << "Could not allocate video frame";
        exit(1);
    }

    frame->format = codecContext->pix_fmt;
    frame->width  = codecContext->width;
    frame->height = codecContext->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        qDebug() << "Could not allocate the video frame data";
        exit(1);
    }
}

void VideoRecorder::initOutputFile(const char* filename) {
    // Print detailed information about the output format
    av_dump_format(formatContext, 0, filename, 1);

    // Open output file
    ret = avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE);
    if (ret < 0) {
        qDebug() << "Failed open file: " << ret;
        exit(1);
    }
}


void VideoRecorder::open(const char* filename, int framerate, int width, int height) {
    // Initialize buffer for this resolution
    pixels = new uint8_t[width * height * 4];

    // Container format, eg: .mp4
    initFormat(filename, framerate);

    // Video codec, eg: h264
    initCodec(width, height);

    initFrame();
    initOutputFile(filename);

    // Write the container header
    ret = avformat_write_header(formatContext, NULL);
    if (ret < 0) {
        qDebug() << "Failed writing container header " << ret;
        exit(1);
    }
}

void VideoRecorder::writeFrame() {
    av_init_packet(packet);
    packet->data = NULL;
    packet->size = 0;

    ret = avcodec_send_frame(codecContext, frame);
    if (ret < 0) {
        qDebug() << "Error sending a frame for encoding" << ret;
        exit(1);
    }

    ret = 1;
    while (ret >= 0) {
        ret = avcodec_receive_packet(codecContext, packet);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        }
        else if (ret < 0) {
            qDebug() << "Error during encoding";
            exit(1);
        }

        // Rescale output packet timestamp values from codec to stream timebase
        av_packet_rescale_ts(packet, codecContext->time_base, stream->time_base);

        ret = av_interleaved_write_frame(formatContext, packet);
        if (ret < 0) {
            qDebug() << "Error while writing video frame: " << ret;
            exit(1);
        }

        av_packet_unref(packet);
    }
}

void VideoRecorder::write(int framenumber) {
    frame->pts = framenumber;

    if (av_frame_make_writable(frame) < 0) {
        exit(1);
    }

    // Opengl format must be converted to yuv420p so that
    // it can be encoded in the video stream
    // Opengl coordinates start in bottom left so the frame
    // must also be flipped to appear correctly
    const int in_linesize[1] = { 4 * codecContext->width };

    sws_scale(swsContext,
        (const uint8_t * const *)&pixels,
        in_linesize, 0, codecContext->height,
        frame->data, frame->linesize);

    writeFrame();
}

void VideoRecorder::close() {
    // Write the rest of the packages
    frame = NULL;
    writeFrame();

    // Finish the video file
    av_write_trailer(formatContext);

    // Close and free stuff
    avio_closep(&formatContext->pb);
    avcodec_close(codecContext);

    delete[] pixels;

    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_free_context(formatContext);
    sws_freeContext(swsContext);
}
