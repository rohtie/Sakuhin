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

        int getLastFrame();

        QOpenGLFramebufferObject* fbo = nullptr;
        QOpenGLFramebufferObject* previewFbo = nullptr;
        QOpenGLFramebufferObject* mainFbo = nullptr;

    signals:
        void idChanged();
        void thumbnailChanged();

    private:
        QOpenGLShaderProgram program;

        int id;
        QString thumbnail;

        bool isPreview = false;
        double time = 0.0;
};

#endif // SHADER_H
