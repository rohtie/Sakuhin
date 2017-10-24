#include "window.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

#include "backend.h"

static GLfloat const rectangle[] = {
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

Window::Window(BackEnd *_backend) {
    backend = _backend;

    sessionPath = "sessions/" + backend->getSessionID() + "/session.glsl";
    fileWatcher.addPath(sessionPath);

    QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
                     this, &Window::onSessionFileChange);
}

QString Window::buildShader() {
    // TODO: Use QTextStream and cache static files
    QFile header(":/header.glsl");
    header.open(QIODevice::ReadOnly);

    QFile session(sessionPath);
    session.open(QIODevice::ReadOnly);
    sessionContents = session.readAll();

    QFile footer(":/footer.glsl");
    footer.open(QIODevice::ReadOnly);

    return (
        QString(header.readAll()) +
        QString(sessionContents) +
        QString(footer.readAll())
    );
}

void Window::recompileShader() {
    QOpenGLShader *oldShader = shader.shaders()[1];

    shader.removeShader(oldShader);

    QString shaderCode = buildShader();

    if (!shader.addShaderFromSourceCode(QOpenGLShader::Fragment, shaderCode)) {
        qDebug() << shader.log();

        shader.addShader(oldShader);
    };

    shader.link();
}

void Window::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));


    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 450 core\n"
        "layout(location = 0) in vec3 position;\n"

        "void main() {\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "}\n"
    );

    // TODO: Find out if it is better to only compile the session glsl and link
    // it together with header and footer files, instead of concatenating them
    shader.addShaderFromSourceCode(QOpenGLShader::Fragment, buildShader());

    shader.link();

    shader.bind();
        vbo.create();
        vbo.bind();
            vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo.allocate(rectangle, sizeof(rectangle));

            vao.create();
            vao.bind();
                shader.enableAttributeArray(0);
                shader.setAttributeBuffer(0, GL_FLOAT, 0, 3);
            vao.release();
        vbo.release();
    shader.release();
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.bind();
    shader.setUniformValue("resolution", width(), height());

        vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.release();
    shader.release();

    update();
}

void Window::onSessionFileChange(const QString &path) {
    QFileInfo fileInfo(path);

    if (fileInfo.lastModified() > lastSessionModification) {
        lastSessionModification = fileInfo.lastModified();

        QFile session(sessionPath);
        session.open(QIODevice::ReadOnly);
        QByteArray newSessionContents = session.readAll();

        if (newSessionContents != sessionContents) {
            sessionContents = newSessionContents;

            recompileShader();
        }
    }

    // On some systems, files are replaced by an entirely new file when
    // modified. thus we need to add it to the file watcher again.
    if(!fileWatcher.files().contains(path) && fileInfo.exists()) {
        fileWatcher.addPath(path);
    }
}
