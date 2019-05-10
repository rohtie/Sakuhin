#include "shader.h"

#include <QString>
#include <QFile>
#include <QJsonArray>

#include "channel.h"
#include "slider.h"

Shader::Shader(const QString filepath, const QString id, int index) {
    this->filepath = filepath;
    this->id = id;
    this->index = index;
    this->thumbnail = thumbnail;

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");

    QFile session(filepath);
    session.open(QIODevice::ReadOnly);

    // TODO: Find out if it is better to only compile the session glsl and link
    // it together with header and footer files, instead of concatenating them
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, build(session.readAll()));
    program.link();

    program.bind();
        program.setUniformValue("channel0", 0);
        program.setUniformValue("channel1", 1);
        program.setUniformValue("channel2", 2);
        program.setUniformValue("channel3", 3);
        program.setUniformValue("channel4", 4);
    program.release();

    for (int i = 0; i < 5; i++) {
        channels.append(new Channel(i, this));
    }
    emit channelsChanged();

    for (int i = 0; i < 4; i++) {
        sliders.append(new Slider());
    }
    emit slidersChanged();

    fbo = mainFbo;
}

void Shader::setPreview(bool isPreview) {
    if (isPreview == this->isPreview) {
        return;
    }

    this->isPreview = isPreview;

    if (isPreview) {
        fbo = previewFbo;
    }
    else {
        fbo = mainFbo;
    }
}

QString Shader::build(QByteArray shaderCode) {
    // TODO: Use QTextStream and cache static files
    QFile header(":/header.glsl");
    header.open(QIODevice::ReadOnly);

    QFile footer(":/footer.glsl");
    footer.open(QIODevice::ReadOnly);

    return (
        QString(header.readAll()) +
        QString(shaderCode) +
        QString(footer.readAll())
    );
}

bool Shader::recompile(QByteArray shaderCode) {
    QOpenGLShader* oldShader = program.shaders()[1];
    program.removeShader(oldShader);

    QString code = build(shaderCode);

    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, code)) {
        program.addShader(oldShader);
        program.link();

        return false;
    }

    program.link();

    program.bind();
        program.setUniformValue("channel0", 0);
        program.setUniformValue("channel1", 1);
        program.setUniformValue("channel2", 2);
        program.setUniformValue("channel3", 3);
        program.setUniformValue("channel4", 4);
    program.release();

    justRecompiled = true;

    return true;
}

void Shader::bind() {
    program.bind();

    for (int i = 0; i < 5; i++) {
        Channel* channel = (Channel*) channels[i];
        channel->bind();
    }
}

void Shader::release() {
    program.release();
}

void Shader::setResolution(int width, int height) {
    if (fbo[pingPongIndex] == nullptr || (width != this->width() || height != this->height())) {
        fbo[pingPongIndex] = new QOpenGLFramebufferObject(width, height);
    }
}

void Shader::setTime(double time) {
    this->time = time;
}

void Shader::setUniformValues() {
    program.setUniformValue("resolution", width(), height());
    program.setUniformValue("time", (GLfloat) time);

    for (int i = 0; i < 4; i++) {
        Slider* slider = (Slider*) sliders.at(i);
        QString sliderName = "slider" + QString::number(i);
        program.setUniformValue(sliderName.toStdString().c_str(), (GLfloat) slider->value);
    }
}

void Shader::createThumbnail(const QString &thumbnailpath) {
    if (fbo[pingPongIndex] == nullptr) {
        return;
    }

    fbo[pingPongIndex]->toImage().save(thumbnailpath, "JPG");
    thumbnail = "file:" + thumbnailpath;

    emit thumbnailChanged();
}

int Shader::width() {
    return fbo[pingPongIndex]->width();
}

int Shader::height() {
    return fbo[pingPongIndex]->height();
}

QOpenGLFramebufferObject* Shader::currentFbo() {
    return fbo[pingPongIndex];
}

void Shader::updatePingPong() {
    previousPingPongIndex = pingPongIndex;
    pingPongIndex = (pingPongIndex + 1) % 2;
}

bool Shader::needsUpdate(qint64 currentTime, bool isPreview) {
    if (justRecompiled) {
        justRecompiled = false;

        return true;
    }

    if (isPreview) {
        if (lastRenderTimePreview != currentTime) {
            lastRenderTimePreview = currentTime;

            return true;
        }
    }
    else {
        if (lastRenderTime != currentTime) {
            lastRenderTime = currentTime;

            return true;
        }
    }

    return false;
}

int Shader::currentFrame() {
    if (fbo[pingPongIndex] == nullptr) {
        return 0;
    }
    else {
        return fbo[pingPongIndex]->texture();
    }
}

int Shader::lastFrame() {
    if (fbo[previousPingPongIndex] == nullptr) {
        return 0;
    }
    else {
        return fbo[previousPingPongIndex]->texture();
    }
}

QJsonObject* Shader::toJson() {
    QJsonObject* jsonShader = new QJsonObject();
    (*jsonShader)["id"] = id;

    QJsonArray* jsonChannels = new QJsonArray();
    for (int i=0; i<channels.count(); i++) {
        Channel* currentChannel = (Channel*) channels.at(i);
        jsonChannels->append(*(currentChannel->toJson()));
    }

    (*jsonShader)["channels"] = *jsonChannels;

    return jsonShader;
}

void Shader::interruptQueueNext() {
    for (int i=0; i<channels.count(); i++) {
        Channel* currentChannel = (Channel*) channels.at(i);
        currentChannel->interruptQueueNext();
    }    
}