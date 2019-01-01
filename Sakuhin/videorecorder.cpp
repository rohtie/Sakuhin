#include "videorecorder.h"

VideoRecorder::VideoRecorder() {

}


void VideoRecorder::open(const QString &filename, int framerate, int width, int height) {
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        qDebug() << "Codec not found";
        exit(1);
    }


    context = avcodec_alloc_context3(codec);
    if (!context) {
        qDebug() << "Could not allocate video codec context";
        exit(1);
    }

    context->width = width;
    context->height = height;
    context->pix_fmt = AV_PIX_FMT_YUV420P;
    context->time_base = (AVRational){1, framerate};
    context->framerate = (AVRational){framerate, 1};

    context->bit_rate = 15000000;
    context->bit_rate_tolerance = 0;
    context->rc_max_rate = 0;
    context->rc_buffer_size = 0;
    context->gop_size = 40;
    context->max_b_frames = 3;
    context->b_frame_strategy = 1;
    context->coder_type = 1;
    context->me_cmp = 1;
    context->me_range = 16;
    context->qmin = 10;
    context->qmax = 51;
    context->scenechange_threshold = 40;
    context->me_subpel_quality = 5;
    context->i_quant_factor = 0.71;
    context->qcompress = 0.6;
    context->max_qdiff = 4;

    buffersize = width * height * 4;


    ret = avcodec_open2(context, codec, NULL);
    if (ret < 0) {
        qDebug() << "Could not open codec";
        exit(1);
    }


    file.setFileName(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open " << filename;
        exit(1);
    }


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

    frame->format = context->pix_fmt;
    frame->width  = context->width;
    frame->height = context->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        qDebug() << "Could not allocate the video frame data";
        exit(1);
    }
}


void VideoRecorder::encodeFrame() {
    av_init_packet(packet);
    packet->data = NULL;
    packet->size = 0;

    if (frame) {
        qDebug() << "Encode frame " << frame->pts;
    }

    ret = avcodec_encode_video2(context, packet, frame, &got_output);

    if (ret < 0) {
        qDebug() << "Error encoding frame " << frame->pts;
        exit(1);
    }

    if (got_output) {
        qDebug() << "Write packet " << packet->pts;
        file.write((const char*) packet->data, packet->size);
        av_packet_unref(packet);
    }
}

void VideoRecorder::write(uint8_t* pixels, int framenumber) {
    frame->pts = framenumber;

    // Convert rgba to yuv420p
    const int in_linesize[1] = { 4 * context->width };

    sws_context = sws_getCachedContext(sws_context,
        context->width, context->height, AV_PIX_FMT_RGBA,
        context->width, context->height, AV_PIX_FMT_YUV420P,
        0, NULL, NULL, NULL);

    sws_scale(sws_context,
        (const uint8_t * const *)&pixels,
        in_linesize, 0, context->height,
        frame->data, frame->linesize);


    encodeFrame();
}

void VideoRecorder::close() {
    // Write the rest of the packages
    frame = NULL;
    do {
        encodeFrame();
    } while (got_output);


    // Write endcode to end of the file so that it is playable mp4 format
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    file.write((const char*) endcode, sizeof(endcode));
    file.close();

    avcodec_free_context(&context);
    av_frame_free(&frame);
    av_packet_free(&packet);
}
