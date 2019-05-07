#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

class VideoGroup;

#include <QObject>
#include <QOpenGLTexture>
#include <QString>

class VideoManager : public QObject {
    Q_OBJECT

    public:
        VideoManager(const QString &fileUrl);
        void next(bool isInterrupting);
        void previous();
        void interruptNext();
        void interruptPrevious();


        QOpenGLTexture* currentFrame();

        void destroy();

        bool isPlaying = true;

    private:
        VideoGroup* rootGroup = nullptr;
        VideoGroup* currentGroup = nullptr;

    signals:

    public slots:
};

#endif // VIDEOMANAGER_H
