#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QOpenGLTexture>
#include <QImage>

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

        explicit Channel(const int &channelLocation);

        void bind();
        void setTexture(QString &fileUrl);
        void setShader(Shader* shader);

    private:
        ChannelType channelType;

        int channelLocation;
        QImage image;
        QOpenGLTexture* texture;

        Shader* shader;
};

#endif // CHANNEL_H
