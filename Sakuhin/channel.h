#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QOpenGLTexture>

#include "shader.h"

class Channel : public QObject {
    Q_OBJECT

    public:
        enum ChannelType {
            TextureType,
            ShaderType,
            HardwareType
        };
        Q_ENUM(ChannelType)

        explicit Channel(QObject* parent = nullptr);

        void bind();
        void setTexture(QString &fileUrl);
        void setShader(Shader* shader);

    private:
        int channelLocation;
        ChannelType channelType;

        QOpenGLTexture* texture;
        Shader* shader;
};

#endif // CHANNEL_H
