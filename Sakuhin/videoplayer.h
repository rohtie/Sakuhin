#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

#include <QString>
#include <QOpenGLTexture>

class VideoPlayer {
    public:
        VideoPlayer(const char* filename);
        QOpenGLTexture* currentFrame();

        QOpenGLTexture* texture;

        AVFormatContext* formatContext = NULL;

        AVCodec* codec;
        AVCodecContext* codecContext;

        AVStream* stream;

        AVPacket* packet;
        AVFrame* frame;

        struct SwsContext* swsContext;

        int videoStream;

        int isFrameComplete = 0;

        uint8_t* pixels;
};

#endif // VIDEOPLAYER_H
