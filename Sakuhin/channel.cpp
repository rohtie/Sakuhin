#include <QOpenGLTexture>

#include "channel.h"

Channel::Channel(int channelLocation) {
    this->channelLocation = channelLocation;
}

void Channel::setTexture(const QString &fileUrl) {
    channelType = TextureType;
    texture = new QOpenGLTexture(QImage(fileUrl).mirrored());
}

void Channel::setShader(Shader* shader) {
    channelType = ShaderType;
    this->shader = shader;
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
        glBindTexture(GL_TEXTURE_2D, shader->lastFrame());
        return;
    }
}
