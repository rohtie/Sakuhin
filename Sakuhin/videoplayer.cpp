#include "videoplayer.h"
#include "videogroup.h"

#include <QString>
#include <QDebug>

VideoPlayer::VideoPlayer(VideoGroup* parent, const QString &filename) {
//     this->parent = parent;
//     this->filename = filename;
}

void VideoPlayer::create() {
//     av_register_all();

//     avformat_open_input(&formatContext, filename.toStdString().c_str(), NULL, NULL);
//     avformat_find_stream_info(formatContext, NULL);

//     // Only get video stream
//     for (int i=0; i<formatContext->nb_streams; i++) {
//         if (formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
//             videoStream = i;
//             break;
//         }
//     }

//     stream = formatContext->streams[videoStream];

//     codecContext = stream->codec;

//     codec = avcodec_find_decoder(codecContext->codec_id);
//     avcodec_open2(codecContext, codec, NULL);

//     frame = av_frame_alloc();
//     packet = av_packet_alloc();

//     swsContext = sws_getCachedContext(NULL,
//         codecContext->width, codecContext->height, codecContext->pix_fmt,
//         codecContext->width, codecContext->height, AV_PIX_FMT_RGBA,
//         0, NULL, NULL, NULL);


//     pixels = new uint8_t[codecContext->width * codecContext->height * 4];

//     texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
//     texture->setSize(codecContext->width, codecContext->height);
//     texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
//     texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
//     texture->create();

//     timer = new QTimer(this);
//     connect(timer, SIGNAL(timeout()), this, SLOT(fetchNextFrame()));

//     // Grab first frame so that it isn't filled with garbage
//     fetchNextFrame();
}

void VideoPlayer::destroy() {
//     texture->destroy();

//     delete[] pixels;

//     av_frame_free(&frame);

//     av_packet_free(&packet);

//     avcodec_close(codecContext);

//     avformat_close_input(&formatContext);

//     sws_freeContext(swsContext);
}

void VideoPlayer::start() {
//     double frametime = 1.0 / av_q2d(stream->r_frame_rate);
//     frametime *= 1000.0;

//     timer->start(frametime);
}

void VideoPlayer::stop() {
//     timer->stop();
}

void VideoPlayer::togglePlay() {
//     if (timer->isActive()) {
//         timer->stop();
//     }
//     else {
//         timer->start();
//     }
}

void VideoPlayer::fetchNextFrame() {
//     if (av_read_frame(formatContext, packet) >= 0) {
//         if (packet->stream_index == videoStream) {
//             avcodec_decode_video2(codecContext, frame, &isFrameComplete, packet);

//             if (isFrameComplete) {
//                 needsUpdate = true;
//             }
//         }
//     }
//     else {
//         timer->stop();
//         parent->childHasFinishedPlaying();
//     }
}

QOpenGLTexture* VideoPlayer::currentFrame() {
//     if (needsUpdate) {
//         const int out_linesize[1] = { 4 * codecContext->width };

//         sws_scale(swsContext,
//             frame->data, frame->linesize,
//             0, codecContext->height,
//             &pixels, out_linesize);

//         texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, pixels);
//         needsUpdate = false;
//     }

//     return texture;
}
