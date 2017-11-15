#include <QOpenGLTexture>

#include "channel.h"

Channel::Channel(const int &channelLocation) {
    this->channelLocation = channelLocation;
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
