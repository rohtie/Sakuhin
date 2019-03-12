#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QJsonObject>

class Shader : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString thumbnail MEMBER thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QList<QObject*> channels MEMBER channels NOTIFY channelsChanged)
    Q_PROPERTY(QList<QObject*> sliders MEMBER sliders NOTIFY slidersChanged)

    public:
        Shader(QString filepath);

        void setPreview(bool isPreview);

        QString build(QByteArray shaderCode);
        bool recompile(QByteArray shaderCode);

        void bind();
        void release();

        void setResolution(int width, int height);
        void setTime(double time);
        void setUniformValues();

        void createThumbnail(const QString &thumbnailpath);

        int width();
        int height();

        QOpenGLFramebufferObject* currentFbo();
        void updatePingPong();

        bool needsUpdate(qint64 currentTime, bool isPreview);

        int currentFrame();
        int lastFrame();

        QJsonObject* toJson();

        QList<QObject*> channels;
        QString filepath;
        QString thumbnail = "qrc:assets/thumbnail.jpg";

        QOpenGLShaderProgram program;

    signals:
        void thumbnailChanged();
        void channelsChanged();
        void slidersChanged();

    private:

        QList<QObject*> sliders;

        bool isPreview = false;
        double time = 0.0;

        int pingPongIndex = 0;
        int previousPingPongIndex = 1;

        bool justRecompiled = false;
        qint64 lastRenderTime = 0;
        qint64 lastRenderTimePreview = 0;

        QOpenGLFramebufferObject** fbo;
        QOpenGLFramebufferObject* previewFbo[2] = {nullptr};
        QOpenGLFramebufferObject* mainFbo[2] = {nullptr};
};

#endif // SHADER_H
