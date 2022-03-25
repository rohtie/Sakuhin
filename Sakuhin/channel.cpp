#include <QOpenGLTexture>

#include "channel.h"
#include "shader.h"
#include "audiodevice.h"
// #include "videomanager.h"

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

    // if (videomanager != nullptr && videomanager->isPlaying) {
    //     // We need to stop the current videomanager
    //     // Before we create a new one
    //     videomanager->destroy();
    // }

    // videomanager = new VideoManager(fileUrl);
    // videomanager->next(false);
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
        // if (videomanager->isPlaying) {
        //     QOpenGLTexture* currentFrame = videomanager->currentFrame();

        //     if (currentFrame->isCreated()) {
        //         currentFrame->bind(channelLocation);
        //     }
        // }

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

void Channel::interruptQueueNext() {
    // if (videomanager != nullptr) {
    //     videomanager->next(true);
    // }
}

void Channel::toggleQueuePlay() {
    // if (videomanager != nullptr) {
    //     videomanager->togglePlay();
    // }
}
