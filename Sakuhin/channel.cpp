#include <QOpenGLTexture>

#include "channel.h"

Channel::Channel(int channelLocation) {
    this->channelLocation = channelLocation;
}

void Channel::setTexture(const QString &fileUrl) {
    channelType = TextureType;
    texture = new QOpenGLTexture(QImage(fileUrl).mirrored());
}

void Channel::setAudioDevice(QObject* audioDevice) {
    channelType = AudioType;
    this->audioDevice = (AudioDevice*) audioDevice;
}

void Channel::bind() {
    if (channelType == TextureType) {
        texture->bind(channelLocation);
        return;
    }

    if (channelType == ShaderType) {
        glActiveTexture(GL_TEXTURE0 + channelLocation);
        glBindTexture(GL_TEXTURE_2D, shader->lastFrame());
        return;
    }
}
