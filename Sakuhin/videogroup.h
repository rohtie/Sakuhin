#ifndef VIDEOGROUP_H
#define VIDEOGROUP_H

class VideoPlayer;
class VideoManager;

#include <QObject>
#include <QOpenGLTexture>
#include <QString>

class VideoGroup : public QObject {
    Q_OBJECT

    public:
        enum VideoGroupMode {
            NoMode,
            SequenceMode,
            SequenceLoopMode,
            RandomMode,
            RandomLoopMode
        };
        Q_ENUM(VideoGroupMode)

        VideoGroup(VideoManager* videomanager, VideoGroup* parent, VideoGroupMode mode, const QString path);
        VideoGroup(VideoManager* videomanager, const QString path);

        VideoGroup* next(bool isInterrupt);
        VideoGroup* previous();
        QOpenGLTexture* currentFrame();
        void childHasFinishedPlaying();

        void togglePlay();

    private:
        int index = -1;
        VideoManager* videomanager = nullptr;
        VideoGroup* parent = nullptr;
        QList<QObject*> children;
        VideoGroupMode mode = NoMode;

        VideoGroup* currentGroup = nullptr;
        VideoPlayer* currentVideo = nullptr;

    signals:

    public slots:
};

#endif // VIDEOGROUP_H
