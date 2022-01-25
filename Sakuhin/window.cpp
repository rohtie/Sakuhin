#include "window.h"

#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QtMath>

#include "backend.h"
#include "channel.h"
#include "shader.h"
#include "shadermanager.h"

static GLfloat const rectangle[] = {
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

Window::Window() {

}

void Window::initialize(Backend* backend, ShaderManager* shadermanager, SceneManager* scenemanager, bool isMaster, bool isPreview) {
    this->backend = backend;
    this->shadermanager = shadermanager;
    this->scenemanager = scenemanager;
    this->isMaster = isMaster;
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
        screenShader.setUniformValue("screenTexture", 0);
    screenShader.release();

    // Used for drawing shaders to the whole screen of a window
    rectangleVao.create();
    rectangleVao.bind();
        rectangleVertexBuffer.create();
        rectangleVertexBuffer.bind();
            rectangleVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            rectangleVertexBuffer.allocate(rectangle, sizeof(rectangle));

            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_TRUE, 0, 0);
        rectangleVertexBuffer.release();
    rectangleVao.release();

    time.start();
}

void Window::drawRectangle() {
    rectangleVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    rectangleVao.release();
}

void Window::render(Shader* shader) {
    if (!shader->needsUpdate(currentTime, isPreview)) {
        return;
    }

    for (int i = 0; i < 5; i++) {
        Channel* channel = (Channel*) shader->channels[i];

        if (channel->channelType == Channel::ShaderType) {
            if (channel->shader != nullptr && channel->shader != shadermanager->currentShader(isPreview)) {
                render(channel->shader);
            }
        }
    }

    shader->updatePingPong();
    shader->setPreview(isPreview);

    shader->setResolution(width(), height());

    if (isRecording) {
        shader->setTime(recordingStartTime + (recordingFrame / recordingFramerate));
    }
    else {
        shader->setTime(currentTime / 1000.0f);
    }

    QOpenGLFramebufferObject* fbo = shader->currentFbo();
    fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
            shader->setUniformValues();

            // Give every shader access to the current shader's frame
            // to be used for more consistent feedback based effects
            glActiveTexture(GL_TEXTURE0 + 5);
            if (shader == shadermanager->currentShader(isPreview)) {
                glBindTexture(GL_TEXTURE_2D, shadermanager->currentShader(isPreview)->lastFrame());                
            }
            else {
                glBindTexture(GL_TEXTURE_2D, shadermanager->currentShader(isPreview)->currentFrame());                
            }

            drawRectangle();
        shader->release();
    fbo->release();
}

void Window::renderScreen(Shader* shader) {
    if (isMaster) {
        render(shader);
    }
    else if (isPreview && !shadermanager->previewIsMain()) {
        render(shader);
    }

    if (shader->currentFbo() == nullptr) {
        return;
    }

    screenShader.bind();
        glClear(GL_COLOR_BUFFER_BIT);

        float scaledWidth = (float) width();
        float scaledHeight = (float) height();
        float centerX = 0.0;
        float centerY = 0.0;

        if (isPreview && shadermanager->previewIsMain()) {
            float scaleX = 1.0;
            float scaleY = 1.0;

            if (shader->width() > shader->height()) {
                scaleY = (float) shader->width() / (float) shader->height();
            }
            else {
                scaleX = (float) shader->height() / (float) shader->width();
            }

            scaledWidth /= scaleX;
            scaledHeight /= scaleY;

            centerX = ((float) width() - scaledWidth) * 0.5;
            centerY = ((float) height() - scaledHeight) * 0.5;
        }

        screenShader.setUniformValue("resolution", scaledWidth, scaledHeight);
        screenShader.setUniformValue("centerAdjustment", centerX, centerY);

        if (isRecording) {
            screenShader.setUniformValue("isFlipped", 1.0f);
        }
        else {
            screenShader.setUniformValue("isFlipped", 0.0f);
        }


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shader->currentFrame());

        drawRectangle();

        if (isRecording) {
            glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, videoRecorder.pixels);

            videoRecorder.write(recordingFrame);

            handleRecordingTime();
        }

        if (isTakingScreenshot) {
            screenshotNumber += 1;

            uint8_t* pixels = new uint8_t[width() * height() * 4];
            glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            QImage screenshot(pixels, width(), height(), QImage::Format_RGBA8888);

            QString screenshotFilename = QString("screenshot_%1_%2.jpg").arg(shadermanager->currentShader(isPreview)->id).arg(screenshotNumber, 4, 10, QLatin1Char('0'));
            screenshot.save(screenshotFilename, "JPG");
            delete[] pixels;

            isTakingScreenshot = false;
        }

    screenShader.release();
}

void Window::processTime() {
    currentTime = time.elapsed();
    performanceTime = currentTime;
}


void Window::setupRecordingTime() {
    recordingFrame = 0.0;
    recordingStartTime = time.elapsed() / 1000.0f;
}


void Window::handleRecordingTime() {
    recordingFrame += 1;

    // Record for 1 minute
    if (recordingFrame > 1800) {
        isRecording = false;
        videoRecorder.close();
    }
}

void Window::paintGL() {
    processTime();

    if (isMaster) {
        updatePerformanceInformation();
    }

    renderScreen(shadermanager->currentShader(isPreview));
    update();
}

void Window::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_F2:
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
            break;

        case Qt::Key_R:
            if (!isRecording) {
                videoRecorder.open("/tmp/out.mp4", recordingFramerate, width(), height());

                setupRecordingTime();
            }
            else {
                videoRecorder.close();
            }

            isRecording = !isRecording;
            break;

        case Qt::Key_F12:
            isTakingScreenshot = true;
            break;
    }
}

void Window::updatePerformanceInformation() {
    double timeSinceLastTime = double(performanceTime - lastTime);
    frameCounter++;

    if (timeSinceLastTime >= 1000) {

        // Approximate time elapsed to 1 second for more stable numbers
        timeSinceLastTime = 1000.0f;

        double ms = timeSinceLastTime / frameCounter;
        double fps = frameCounter * 1000 / timeSinceLastTime;

        // TODO: Send performance information through the window manager instead
        backend->setPerformanceInformation(QString::number(ms) + " ms " + QString::number(fps) + " fps");

        frameCounter = 0;
        lastTime += 1000;
    }
}


void Window::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage){
    qDebug() << (isPreview ? "Preview: " : "Main: ") << debugMessage.message();
}
