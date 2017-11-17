#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>

#include "backend.h"
#include "channel.h"

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

    #ifdef QT_DEBUG
    qDebug() << reinterpret_cast<const char*>(glGetString(GL_VERSION));

    QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged,
            this, &Window::handleLoggedMessage);
    logger->startLogging();
    #endif

    screenShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
    screenShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/screenShader.glsl");
    screenShader.link();

    screenShader.bind();
        screenShader.setUniformValue("screenTexture", GL_TEXTURE0);
    screenShader.release();

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

void Window::render(Shader* shader) {
    shader->updatePingPong();

    shader->setPreview(isPreview);
    shader->setResolution(width(), height());
    shader->setTime(time.elapsed() / 1000.0f);

    QOpenGLFramebufferObject* fbo = shader->currentFbo();
    fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
            shader->setUniformValues();

            drawRectangle();
        shader->release();
    fbo->release();
}

void Window::renderScreen(Shader* shader) {
    if (!isPreview || !shadermanager->previewIsMain()) {
        render(shader);
    }

    screenShader.bind();
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.setUniformValue("resolution", width(), height());
        screenShader.setUniformValue("screenTextureResolution", shader->width(), shader->height());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shader->currentFrame());

        drawRectangle();
    screenShader.release();
}

void Window::paintGL() {
    if (!isPreview) {
        updatePerformanceInformation();
    }

    renderScreen(shadermanager->currentShader(isPreview));
    update();
}

void Window::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F2) {
        if ((flags() & Qt::FramelessWindowHint) == 0) {
            oldGeometry = frameGeometry();
            oldMargins = frameMargins();

            int frameWidth = oldGeometry.width();
            int frameHeight = oldGeometry.height();

            QPoint oldPosition = framePosition();

            setFlags(Qt::Window | Qt::FramelessWindowHint);

            setWidth(frameWidth);
            setHeight(frameHeight);
            setPosition(oldPosition);
        }
        else {
            setFlags(Qt::Window);

            // TODO: Get the default frame margins when window is already frameless
            if (oldMargins.top() == 0) {
                oldGeometry = frameGeometry();
            }

            oldGeometry.adjust(
                oldMargins.left(),
                oldMargins.top(),
                -oldMargins.right(),
                -oldMargins.bottom()
            );

            setGeometry(oldGeometry);
        }
    }
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


void Window::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage){
    qDebug() << (isPreview ? "Preview: " : "Main: ") << debugMessage.message();
}
