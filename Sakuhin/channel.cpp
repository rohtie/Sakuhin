#include <QOpenGLTexture>

#include "channel.h"

Channel::Channel(int channelLocation, Shader* owner) {
    this->owner = owner;
    this->channelLocation = channelLocation;
}

void Channel::setTexture(const QString &fileUrl) {
    channelType = TextureType;
    texture = new QOpenGLTexture(QImage(fileUrl).mirrored());

    thumbnail = "file:" + fileUrl;
    emit thumbnailChanged();
}

void Channel::setShader(Shader* shader) {
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
}

void Channel::fetchShaderThumbnail() {
    thumbnail = shader->thumbnail;
    emit thumbnailChanged();
}