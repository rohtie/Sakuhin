#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <QObject>
#include <QFile>
#include <QDebug>

class VideoRecorder : public QObject {
    Q_OBJECT

    public:
        VideoRecorder();
        void open(const QString &filename, int framerate, int width, int height);
        void encodeFrame();
        void write(uint8_t* pixels, int framenumber);
        void close();

    private:
        int ret;
        int got_output;

        QFile file;

        int buffersize;

        AVCodec* codec;
        AVCodecContext* context;
        AVPacket* packet;
        AVFrame* frame;
        struct SwsContext* sws_context;
        uint8_t* rgb;
};

#endif // VIDEORECORDER_H
