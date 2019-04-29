#include "videoplayer.h"

#include <QString>
#include <QDebug>

VideoPlayer::VideoPlayer(const char* filename) {
    av_register_all();

    avformat_open_input(&formatContext, filename, NULL, NULL);
    avformat_find_stream_info(formatContext, NULL);

    // Only get video stream
    for (int i=0; i<formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    codecContext = formatContext->streams[videoStream]->codec;

    codec = avcodec_find_decoder(codecContext->codec_id);
    avcodec_open2(codecContext, codec, NULL);

    frame = av_frame_alloc();
    packet = av_packet_alloc();

    swsContext = sws_getCachedContext(NULL,
        codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGBA,
        0, NULL, NULL, NULL);


    pixels = new uint8_t[codecContext->width * codecContext->height * 4];

    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setSize(codecContext->width, codecContext->height);
    texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
    texture->create();
}

QOpenGLTexture* VideoPlayer::currentFrame() {
    if (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStream) {
            avcodec_decode_video2(codecContext, frame, &isFrameComplete, packet);

            if (isFrameComplete) {
                const int out_linesize[1] = { 4 * codecContext->width };

                sws_scale(swsContext,
                    frame->data, frame->linesize,
                    0, codecContext->height,
                    &pixels, out_linesize);

                texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, pixels);
            }
        }
    }

    return texture;
}
