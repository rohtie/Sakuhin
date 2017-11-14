#include "window.h"
#include <QDebug>
#include <QString>
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

Window::Window(BackEnd* backend, ShaderManager* shadermanager, bool isPreview) {
    this->backend = backend;
    this->shadermanager = shadermanager;
    this->isPreview = isPreview;
}

void Window::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));

    #ifdef QT_DEBUG
    QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged,
            this, &Window::handleLoggedMessage);
    logger->startLogging();
    #endif

    shader = shadermanager->mainShader;

    vao.create();
    vao.bind();
        vbo.create();
        vbo.bind();
            vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo.allocate(rectangle, sizeof(rectangle));

            glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_TRUE, 0, 0);
            glEnableVertexAttribArray(0);
        vbo.release();
    vao.release();

    time.start();
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

void Window::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage){
    qDebug() << (isPreview ? "Preview: " : "Main: ") << debugMessage.message();
}
