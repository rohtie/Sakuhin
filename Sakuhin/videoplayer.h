#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

class VideoGroup;

#include <QString>
#include <QOpenGLTexture>
#include <QObject>
#include <QTimer>

class VideoPlayer : public QObject {
    Q_OBJECT

    public:
        VideoPlayer(VideoGroup* parent, const QString &filename);
        void create();
        void destroy();
        void start();
        void stop();
        QOpenGLTexture* currentFrame();

    private:
        VideoGroup* parent;
        QString filename;

        QOpenGLTexture* texture;
        QTimer* timer;
        bool needsUpdate = false;

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

    public slots:
        void fetchNextFrame();

};

#endif // VIDEOPLAYER_H
