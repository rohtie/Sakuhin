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
            NoType,
            TextureType,
            ShaderType,
            HardwareType
        };
        Q_ENUM(ChannelType)

        explicit Channel(int channelLocation);

        void bind();
        Q_INVOKABLE void setTexture(const QString &fileUrl);
        void setShader(Shader* shader);

        ChannelType channelType = NoType;
        Shader* shader;

    private:
        int channelLocation = 0;
        QOpenGLTexture* texture;

};

#endif // CHANNEL_H
