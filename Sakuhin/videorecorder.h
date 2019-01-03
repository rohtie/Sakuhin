#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}

#include <QObject>
#include <QFile>
#include <QDebug>

class VideoRecorder : public QObject {
    Q_OBJECT

    public:
        VideoRecorder();
        void initFormat(const char* filename, int framerate);
        void initCodec(int width, int height);
        void initFrame();
        void initOutputFile(const char* filename);
        void open(const char* filename, int framerate, int width, int height);
        void writeFrame();
        void write(uint8_t* pixels, int framenumber);
        void close();

    private:
        int ret;

        const QString filename;
        int framerate;

        AVOutputFormat* format;
        AVFormatContext* formatContext;

        AVCodec* codec;
        AVCodecContext* codecContext;

        AVStream* stream;

        AVPacket* packet;
        AVFrame* frame;

        struct SwsContext* swsContext;
};

#endif // VIDEORECORDER_H
