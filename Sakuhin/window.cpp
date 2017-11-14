#include "window.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>
#include <QOpenGLDebugLogger>

#include "backend.h"

static GLfloat const rectangle[] = {
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

Window::Window(BackEnd* backend,
               ShaderManager* shadermanager,
               bool isPreview) {

    this->backend = backend;
    this->shadermanager = shadermanager;
    this->isPreview = isPreview;

    if (isPreview) {
        fileWatcher.addPath(
            "sessions/" + backend->getSessionID() + "/session.glsl"
        );

        QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
                         this, &Window::onSessionFileChange);
    }
}

void Window::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));

    #ifdef QT_DEBUG
    QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &Window::handleLoggedMessage);
    logger->startLogging();
    #endif

    if (isPreview) {
        shadermanager->initializeGL();
        shader = shadermanager->previewShader;
    }
    else {
        shader = shadermanager->mainShader;
    }

    vbo.create();
    vbo.bind();
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo.allocate(rectangle, sizeof(rectangle));
        vao.create();
    vbo.release();

    bindVAO();

    time.start();
}

void Window::bindVAO() {
    vbo.bind();
        vao.bind();
            shader->bindVAO();
        vao.release();
    vbo.release();
}

void Window::drawRectangle() {
    vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.release();
}

void Window::render() {
    shader->setPreview(isPreview);
    shader->setResolution(width(), height());
    shader->setTime(time.elapsed() / 1000.0f);

    shader->bind();
        shader->setUniformValues();

        drawRectangle();
    shader->release();
}

void Window::updatePerformanceInformation() {
    qint64 currentTime = time.elapsed();
    double timeSinceLastTime = double(currentTime - lastTime);
    frameCounter++;

    if (timeSinceLastTime >= 1000) {

        // Approximate time elapsed to 1 second for more stable numbers
        timeSinceLastTime = 1000.0f;

        double ms = timeSinceLastTime / frameCounter;
        double fps = frameCounter * 1000 / timeSinceLastTime;

        backend->setPerformanceInformation(QString::number(ms) + " ms " + QString::number(fps) + " fps");

        frameCounter = 0;
        lastTime += 1000;
    }
}

void Window::paintGL() {
    if (!isPreview) {
        updatePerformanceInformation();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    render();
    update();
}

void Window::onSessionFileChange(const QString &path) {
    QFileInfo fileInfo(path);

    if (fileInfo.lastModified() > lastSessionModification) {
        lastSessionModification = fileInfo.lastModified();

        QFile session(path);
        session.open(QIODevice::ReadOnly);
        QByteArray newSessionContents = session.readAll();

        if (newSessionContents != sessionContents) {
            sessionContents = newSessionContents;

            shader->recompile(sessionContents);
        }
    }

    // On some systems, files are replaced by an entirely new file when
    // modified. thus we need to add it to the file watcher again.
    if(!fileWatcher.files().contains(path) && fileInfo.exists()) {
        fileWatcher.addPath(path);
    }
}

void Window::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage){
    qDebug() << (isPreview ? "Preview: " : "Main: ") << debugMessage.message();
}
