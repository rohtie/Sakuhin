#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>

class Shader : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id MEMBER id NOTIFY idChanged)
    Q_PROPERTY(QString thumbnail MEMBER thumbnail NOTIFY thumbnailChanged)

    public:
        Shader(int id, QString thumbnail, QString sessionpath);

        void setPreview(bool isPreview);

        QString build(QByteArray shaderCode);
        bool recompile(QByteArray shaderCode);

        void bind();
        void release();

        void setResolution(int width, int height);
        void setTime(double time);
        void setUniformValues();

        int width();
        int height();

        QOpenGLFramebufferObject* currentFbo();
        void updatePingPong();

        int currentFrame();
        int lastFrame();

    signals:
        void idChanged();
        void thumbnailChanged();

    private:
        QOpenGLShaderProgram program;

        int id;
        QString thumbnail;

        bool isPreview = false;
        double time = 0.0;

        int pingPongIndex = 0;
        int previousPingPongIndex = 1;

        QOpenGLFramebufferObject** fbo;
        QOpenGLFramebufferObject* previewFbo[2] = {nullptr};
        QOpenGLFramebufferObject* mainFbo[2] = {nullptr};
};

#endif // SHADER_H
