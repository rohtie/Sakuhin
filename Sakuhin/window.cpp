#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>

#include "objloader.h"

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


Window::Window() {

}

void Window::setupMapping(float distanceFromObject, float projectorHeight, float fieldOfView, bool isVertical, const QString &modelPath) {
    this->distanceFromObject = distanceFromObject;
    this->projectorHeight = projectorHeight;
    this->fieldOfView = fieldOfView;
    this->isVertical = isVertical;
    this->modelPath = modelPath;
    this->isProjectionMapping = true;
}

void Window::initialize(BackEnd* backend, ShaderManager* shadermanager, bool isMaster, bool isPreview) {
    this->backend = backend;
    this->shadermanager = shadermanager;
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

    connect(this, SIGNAL(isProjectionMappingChanged()), this, SLOT(updateProjectionMapping()));
    connect(this, SIGNAL(isVerticalChanged()), this, SLOT(updateMVPmatrix()));
    connect(this, SIGNAL(distanceFromObjectChanged()), this, SLOT(updateMVPmatrix()));
    connect(this, SIGNAL(projectorHeightChanged()), this, SLOT(updateMVPmatrix()));
    connect(this, SIGNAL(fieldOfViewChanged()), this, SLOT(updateMVPmatrix()));

    if (isProjectionMapping) {
        updateProjectionMapping();
    }

    time.start();
}

void Window::updateProjectionMapping() {
    // TODO: Figure out how to seamlessly switch between projection view and shader view
    //       without getting black windows

    if (hasLoadedProjectionObject) {
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    meshShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/meshVertex.glsl");
    meshShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/meshShader.glsl");
    meshShader.link();

    meshShader.bind();
        meshShader.setUniformValue("mainTexture", 0);
    meshShader.release();

    updateMVPmatrix();

    loadMesh(modelPath,
             // Opengl expanded triangle vertices
             meshVertices, meshUVs,
             // Abstract model data
             vertices, UVs, vertexFaces, UVFaces);

    meshVao.create();
    meshVao.bind();
        meshVertexBuffer.create();
        meshVertexBuffer.bind();
            meshVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            meshVertexBuffer.allocate(meshVertices.constData(), meshVertices.size() * sizeof(GLfloat));

            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_TRUE, 0, 0);
        meshVertexBuffer.release();

        meshUVbuffer.create();
        meshUVbuffer.bind();
            meshUVbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            meshUVbuffer.allocate(meshUVs.constData(), meshUVs.size() * sizeof(GLfloat));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer((GLuint) 1, 2, GL_FLOAT, GL_TRUE, 0, 0);
        meshUVbuffer.release();
    meshVao.release();

    hasLoadedProjectionObject = true;
}

void Window::updateMVPmatrix() {
    QVector3D upVector(0, 1, 0);
    if (isVertical) {
        upVector = QVector3D(1, 0, 0);
    }

    viewMatrix.setToIdentity();
    projectionMatrix.setToIdentity();
    modelMatrix.setToIdentity();
    mvpMatrix.setToIdentity();

    viewMatrix.lookAt(QVector3D(0, projectorHeight, distanceFromObject), QVector3D(0, projectorHeight, 0), upVector);
    projectionMatrix.perspective(fieldOfView, (float) width() / (float) height(), 0.1f, 1000.0f);
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}

void Window::resizeGL(int width, int height) {
    updateMVPmatrix();
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
    if (isMaster) {
        render(shader);
    }
    else if (isPreview && !shadermanager->previewIsMain()) {
        render(shader);
    }

    if (isProjectionMapping && hasLoadedProjectionObject) {
        meshShader.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            meshShader.setUniformValue("mvpMatrix", mvpMatrix);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, shader->currentFrame());

            // The fbo doesn't care about filtering the texture, so we have to do it manually
            // TODO: Find out if it is possible to do this in a
            //       different way as it takes about 0.25 ms to compute
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            // glGenerateMipmap(GL_TEXTURE_2D);

            meshVao.bind();
                glDrawArrays(GL_TRIANGLES, 0, meshVertices.size());
            meshVao.release();
        meshShader.release();
    }
    else {
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

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, shader->currentFrame());

            drawRectangle();
        screenShader.release();
    }
}

void Window::paintGL() {
    currentTime = time.elapsed();

    if (isMaster) {
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
    double timeSinceLastTime = double(currentTime - lastTime);
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
