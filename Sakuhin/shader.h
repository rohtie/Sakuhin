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

        void bindVAO();
        void setPreview(bool isPreview);

        QString build(QByteArray shaderCode);
        bool recompile(QByteArray shaderCode);

        void bind();
        void release();

        void setResolution(int width, int height);
        void setTime(double time);
        void setUniformValues();

        int getLastFrame();

        QOpenGLShaderProgram program;

    signals:
        void idChanged();
        void thumbnailChanged();

    private:
        int id;
        QString thumbnail;

        bool isPreview = false;
        double time = 0.0;

        QOpenGLFramebufferObject* fbo = nullptr;
        QOpenGLFramebufferObject* previewFbo = nullptr;
        QOpenGLFramebufferObject* mainFbo = nullptr;

};

#endif // SHADER_H
