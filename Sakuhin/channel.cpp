#include <QOpenGLTexture>

#include "channel.h"
#include "shader.h"
#include "audiodevice.h"
#include "videoplayer.h"

Channel::Channel(int channelLocation, Shader* owner) {
    this->owner = owner;
    this->channelLocation = channelLocation;
}

void Channel::setTexture(const QString &fileUrl) {
    texturePath = fileUrl;

    channelType = TextureType;
    texture = new QOpenGLTexture(QImage(fileUrl).mirrored());

    thumbnail = "file:" + fileUrl;
    emit thumbnailChanged();
}

void Channel::setShader(QObject* inputShader) {
    Shader* shader = (Shader*) inputShader;

    if (this->shader != nullptr) {
        disconnect(this->shader, SIGNAL(thumbnailChanged()), 0, 0);
    }
    connect(shader, SIGNAL(thumbnailChanged()), this, SLOT(fetchShaderThumbnail()));

    channelType = ShaderType;
    this->shader = shader;

    fetchShaderThumbnail();
}

void Channel::setAudioDevice(QObject* audioDevice) {
    channelType = AudioType;
    this->audioDevice = (AudioDevice*) audioDevice;
    this->audioDevice->start();

    thumbnail = "qrc:assets/audio_icon.jpg";
    emit thumbnailChanged();
}

void Channel::setVideo(const QString &fileUrl) {
    qDebug() << fileUrl;

    if (videoPlayer != nullptr) {
        videoPlayer->stop();
    }

    videoPlayer = new VideoPlayer(fileUrl.toStdString().c_str());
    videoPlayer->start();
    channelType = VideoType;
}

void Channel::bind() {
    if (channelType == TextureType) {
        texture->bind(channelLocation);
        return;
    }
    else if (channelType == AudioType) {
        QOpenGLTexture* spectrum = audioDevice->spectrumTexture();

        if (spectrum->isCreated()) {
            spectrum->bind(channelLocation);
        }

        return;
    }
    else if (channelType == ShaderType) {
        glActiveTexture(GL_TEXTURE0 + channelLocation);

        if (owner == shader) {
            glBindTexture(GL_TEXTURE_2D, shader->lastFrame());
        }
        else {
            glBindTexture(GL_TEXTURE_2D, shader->currentFrame());
        }
        return;
    }
    else if (channelType == VideoType) {
        QOpenGLTexture* currentFrame = videoPlayer->currentFrame();

        if (currentFrame->isCreated()) {
            currentFrame->bind(channelLocation);
        }

        return;
    }
}

void Channel::fetchShaderThumbnail() {
    thumbnail = shader->thumbnail;
    emit thumbnailChanged();
}

QJsonObject* Channel::toJson() {
    QJsonObject* jsonChannel = new QJsonObject();
    (*jsonChannel)["type"] = channelType;

    if (channelType == TextureType) {
        (*jsonChannel)["filename"] = texturePath;
    }
    else if (channelType == AudioType) {
        (*jsonChannel)["device"] = audioDevice->name;
    }
    else if (channelType == ShaderType) {
        (*jsonChannel)["shader"] = shader->index;
    }

    return jsonChannel;
}
