#ifndef CHANNEL_H
#define CHANNEL_H

class Shader;
class AudioDevice;
class VideoManager;

#include <QObject>
#include <QOpenGLTexture>
#include <QImage>
#include <QJsonObject>

class Channel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString thumbnail MEMBER thumbnail NOTIFY thumbnailChanged)

    public:
        enum ChannelType {
            NoType,
            TextureType,
            ShaderType,
            AudioType,
            HardwareType,
            VideoType
        };
        Q_ENUM(ChannelType)

        explicit Channel(int channelLocation, Shader* owner);

        void bind();
        Q_INVOKABLE void setTexture(const QString &fileUrl);
        Q_INVOKABLE void setShader(QObject* inputShader);
        Q_INVOKABLE void setAudioDevice(QObject* audioDevice);
        Q_INVOKABLE void setVideo(const QString &fileUrl);

        QJsonObject* toJson();

        Shader* owner;
        ChannelType channelType = NoType;
        Shader* shader = nullptr;
        QString thumbnail;
        QString texturePath;

    public slots:
        void fetchShaderThumbnail();

    signals:
        void thumbnailChanged();

    private:
        int channelLocation = 0;
        QOpenGLTexture* texture;

        AudioDevice* audioDevice;
        VideoManager* videomanager = nullptr;
};

#endif // CHANNEL_H
