#include <QString>
#include <QFile>

#include "shader.h"

Shader::Shader(int id, QString thumbnail, QString sessionpath) {
    this->id = id;
    this->thumbnail = thumbnail;

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");

    // TODO: Find out if it is better to only compile the session glsl and link
    // it together with header and footer files, instead of concatenating them
    QFile session(sessionpath);
    session.open(QIODevice::ReadOnly);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, build(session.readAll()));
    program.link();

    program.bind();
        program.setUniformValue("channel0", 0);
        program.setUniformValue("channel1", 1);
        program.setUniformValue("channel2", 2);
        program.setUniformValue("channel3", 3);
        program.setUniformValue("channel4", 4);
    program.release();
}

void Shader::setPreview(bool isPreview) {
    if (isPreview == this->isPreview) {
        return;
    }

    this->isPreview = isPreview;

    if (isPreview) {
        mainFbo = fbo;
        fbo = previewFbo;
    }
    else {
        previewFbo = fbo;
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

        qDebug() << program.log();

        return false;
    }

    program.link();
    return true;
}

void Shader::bind() {
    program.bind();
}

void Shader::release() {
    program.release();
}

void Shader::setResolution(int width, int height) {
    if (fbo == nullptr || (width != fbo->width() || height != fbo->height())) {
        fbo = new QOpenGLFramebufferObject(width, height);
    }
}

void Shader::setTime(double time) {
    this->time = time;
}

void Shader::setUniformValues() {
    program.setUniformValue("resolution", fbo->width(), fbo->height());
    program.setUniformValue("time", (GLfloat) time);

    // TODO: Fix sliders
    // shader.setUniformValueArray("slider", (GLfloat*) backend->getSliders(), 4, 1);
}

int Shader::getLastFrame() {
    return fbo->texture();
}
