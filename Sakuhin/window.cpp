#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtMath>
#include <QJsonArray>

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

static GLfloat const calibration[] = {
    -0.01f,  0.01f, 0.0f,
     0.01f,  0.01f, 0.0f,
    -0.01f, -0.01f, 0.0f,

    -0.01f, -0.01f, 0.0f,
     0.01f,  0.01f, 0.0f,
     0.01f, -0.01f, 0.0f
};

static GLfloat const calibrationUVs[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,

    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};


Window::Window() {

}

void Window::setupMapping(float distanceFromObject, float projectorHeight, float fieldOfView, bool isVertical, const QString &modelPath) {
    configPath = "data/config/dinzoil_F.json";

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

    // Used for drawing overlayed sprites
    QOpenGLBuffer calibrationUVbuffer;
    calibrationTexture = new QOpenGLTexture(QImage("data/textures/calibrate.png").mirrored());
    calibrationTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);

    calibrationVao.create();
    calibrationVao.bind();
        calibrationVertexBuffer.create();
        calibrationVertexBuffer.bind();
            calibrationVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            calibrationVertexBuffer.allocate(calibration, sizeof(calibration));

            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_TRUE, 0, 0);
        calibrationVertexBuffer.release();

        calibrationUVbuffer.create();
        calibrationUVbuffer.bind();
            calibrationVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            calibrationVertexBuffer.allocate(calibrationUVs, sizeof(calibrationUVs));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer((GLuint) 1, 2, GL_FLOAT, GL_TRUE, 0, 0);
        calibrationUVbuffer.release();
    calibrationVao.release();


    connect(this, SIGNAL(isProjectionMappingChanged()), this, SLOT(updateProjectionMapping()));
    connect(this, SIGNAL(isVerticalChanged()), this, SLOT(updateProjectionMappingSettings()));
    connect(this, SIGNAL(distanceFromObjectChanged()), this, SLOT(updateProjectionMappingSettings()));
    connect(this, SIGNAL(projectorHeightChanged()), this, SLOT(updateProjectionMappingSettings()));
    connect(this, SIGNAL(objectHeightTargetChanged()), this, SLOT(updateProjectionMappingSettings()));
    connect(this, SIGNAL(fieldOfViewChanged()), this, SLOT(updateProjectionMappingSettings()));

    if (isProjectionMapping) {
        updateProjectionMapping();
    }

    time.start();
}

void Window::updateCalibrationPoints() {

    calibrationPoints.clear();

    for (int i=0; i<vertices.size(); i++) {
        QVector3D* vertex = vertices[i];

        QVector3D projected = (*vertex).project(viewMatrix * modelMatrix, projectionMatrix, QRect(0, 0, width(), height()));
        QVector2D normalized = QVector2D(projected.x() / (float) width(), projected.y() / (float) height());
        QVector2D* screenSpace = new QVector2D(
            ((normalized.x() - 0.5) * 2.0) / ((float) height() / (float) width()),
            (normalized.y() - 0.5) * 2.0
        );

        calibrationPoints.append(screenSpace);
    }
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


    for (int i=0; i<vertices.size(); i++) {
        originalVertices.append(new QVector3D(*vertices[i]));
    }

    updateCalibrationPoints();

    meshVao.create();
    meshVao.bind();
        meshVertexBuffer.create();
        meshVertexBuffer.bind();
            meshVertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
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

void Window::updateMesh() {
    meshVertices.clear();
    meshUVs.clear();

    expandModel(
        meshVertices,
        meshUVs,
        vertices,
        UVs,
        vertexFaces,
        UVFaces
    );

    meshVertexBuffer.bind();
        meshVertexBuffer.write(0, meshVertices.constData(), meshVertices.size() * sizeof(GLfloat));
    meshVertexBuffer.release();
}

void Window::updateProjectionMappingSettings() {
    // Whenever the projection mapping settings change
    // we reset the vertices to their original state
    // so that the model stays true to its orginal form.


    // Vertices have to be reset so that our calibration
    // points will be correct from the current view
    vertices.clear();
    for (int i=0; i<originalVertices.size(); i++) {
        vertices.append(new QVector3D(*originalVertices[i]));
    }

    updateMesh();
    updateMVPmatrix();

    // Camera position / rotation has changed
    // therefore we need to reconstruct the
    // calibration points
    updateCalibrationPoints();
}

void Window::updateMVPmatrix() {
    // We don't need to transform the model
    // since it should always be in the origin
    modelMatrix.setToIdentity();

    // View matrix
    QVector3D camera(0, projectorHeight, distanceFromObject);
    QVector3D target(0, objectHeightTarget, 0);
    QVector3D upVector(0, 1, 0);

    if (isVertical) {
        upVector = QVector3D(1, 0, 0);
    }

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(camera, target, upVector);

    QMatrix4x4 cameraMatrix = viewMatrix.inverted();
    cameraForward = (cameraMatrix * QVector4D(0., 0., 1., 0.)).toVector3D();
    cameraRight = (cameraMatrix * QVector4D(1., 0., 0., 0.)).toVector3D();
    cameraUp = (cameraMatrix * QVector4D(0., 1., 0., 0.)).toVector3D();

    // Projection matrix
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fieldOfView, (float) width() / (float) height(), cameraNear, cameraFar);

    // MVP matrix sent to the vertex shader
    mvpMatrix.setToIdentity();
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    // Matrix used to display calibration points
    calibrationMatrix.setToIdentity();
    calibrationMatrix.scale(QVector3D((float) height() / (float) width(), 1, 1));
}

void Window::resizeGL(int width, int height) {
    updateMVPmatrix();
}

void Window::drawRectangle() {
    rectangleVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    rectangleVao.release();
}

void Window::drawBillboard(float x, float y) {
    calibrationMatrix.translate(QVector3D(x, y, 0.));

    meshShader.setUniformValue("mvpMatrix", calibrationMatrix);
    calibrationVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    calibrationVao.release();

    calibrationMatrix.translate(QVector3D(-x, -y, 0.));
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


        // Calibration overlay
        if (isCalibrating) {
            meshShader.bind();
                calibrationTexture->bind(0);

                for (int i=0; i<calibrationPoints.size(); i++) {
                    QVector2D* calibrationPoint = calibrationPoints[i];
                    drawBillboard(calibrationPoint->x(), calibrationPoint->y());
                }
            meshShader.release();
        }
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

QVector2D* Window::normalizeCoordinates(float x, float y) {
    x /= width();
    y /= height();

    x -= 0.5;
    y -= 0.5;

    x *= 2.0;
    y *= 2.0;

    x /= (float) height() / (float) width();

    return new QVector2D(x, y);
}

void Window::mousePressEvent(QMouseEvent* event) {
    if (isCalibrating) {

        QPointF pos = event->localPos();
        float x = pos.rx();
        float y = height() - pos.ry();
        QVector2D* mousePos = normalizeCoordinates(x, y);

        if (selectedCalibrationPoint != -1) {
            selectedCalibrationPoint = -1;
        }
        else {
            for (int i=0; i<calibrationPoints.size(); i++) {
                QVector2D* calibrationPoint = calibrationPoints[i];

                if (mousePos->distanceToPoint(*calibrationPoint) < 0.025) {
                    selectedCalibrationPoint = i;
                    break;
                }
            }
        }
    }
}

void Window::mouseMoveEvent(QMouseEvent* event) {
    if (isCalibrating) {
        QPointF pos = event->localPos();
        float x = pos.rx();
        float y = height() - pos.ry();
        QVector2D* mousePos = normalizeCoordinates(x, y);

        if (selectedCalibrationPoint > -1) {
            QVector2D* calibrationPoint = calibrationPoints[selectedCalibrationPoint];
            calibrationPoint->setX(mousePos->x());
            calibrationPoint->setY(mousePos->y());

            // Use the projected depth of the selected vertex to determine
            // the new position of the vertex in relation to its calibration point
            QVector3D* vertex = vertices[selectedCalibrationPoint];
            QVector3D transformedVertex = (*vertex).project(viewMatrix * modelMatrix, projectionMatrix, QRect(0, 0, width(), height()));
            (*vertex) = QVector3D(x, y, transformedVertex.z()).unproject(viewMatrix * modelMatrix, projectionMatrix, QRect(0, 0, width(), height()));

            updateMesh();
        }
    }
}

void Window::saveProjectionMappingConfig() {
    QFile configFile(configPath);

    if (!configFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't open save file";
        return;
    }


    QJsonArray jsonCalibrationPoints;

    for (int i=0; i<calibrationPoints.size(); i++) {
        QVector2D* calibrationPoint = calibrationPoints[i];

        QJsonObject jsonCalibrationPoint;
        jsonCalibrationPoint["x"] = calibrationPoint->x();
        jsonCalibrationPoint["y"] = calibrationPoint->y();
        jsonCalibrationPoints.append(jsonCalibrationPoint);
    }


    QJsonObject config;

    config["calibrationPoints"] = jsonCalibrationPoints;
    config["modelPath"] = modelPath;
    config["distanceFromObject"] = distanceFromObject;
    config["projectorHeight"] = projectorHeight;
    config["objectHeightTarget"] = objectHeightTarget;
    config["fieldOfView"] = fieldOfView;
    config["vertical"] = isVertical;


    QJsonDocument configDocument(config);
    configFile.write(configDocument.toJson());
    configFile.close();
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

        case Qt::Key_C:
            isCalibrating = !isCalibrating;
            break;

        case Qt::Key_S:
            if (isProjectionMapping) {
                saveProjectionMappingConfig();
            }
            break;
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
