#include <QOpenGLTexture>

#include "channel.h"

Channel::Channel(QObject* parent) : QObject(parent) {

}

void Channel::bind() {
    if (channelType == TextureType) {
        texture->bind(channelLocation);
        return;
    }

    if (channelType == ShaderType) {
        glActiveTexture(channelLocation);
        glBindTexture(GL_TEXTURE_2D, shader->lastFrame());
        return;
    }
}
