#include "mappingwindow.h"

#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "objloader.h"
#include "shader.h"
#include "shadermanager.h"


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

MappingWindow::MappingWindow() {

}


void MappingWindow::initializeGL() {
    Window::initializeGL();

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
}

void MappingWindow::renderScreen(Shader* shader) {
    if (isMaster) {
        render(shader);
    }
    else if (isPreview && !shadermanager->previewIsMain()) {
        render(shader);
    }

    if (hasLoadedProjectionObject) {
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
}

void MappingWindow::setupMapping(const QString &configPath) {
    this->configPath = configPath;

    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open config file";
        return;
    }

    QByteArray data = configFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);
    configFile.close();


    this->modelPath = document["modelPath"].toString();

    this->distanceFromObject = document["distanceFromObject"].toDouble();
    this->projectorHeight = document["projectorHeight"].toDouble();
    this->objectHeightTarget = document["objectHeightTarget"].toDouble();
    this->fieldOfView = document["fieldOfView"].toDouble();
    this->isVertical = document["isVertical"].toBool();

    QJsonArray jsonCalibrationPoints = document["calibrationPoints"].toArray();

    for (int i=0; i<jsonCalibrationPoints.size(); i++) {
        QJsonObject jsonCalibrationPoint = jsonCalibrationPoints[i].toObject();
        calibrationPoints.append(new QVector2D(jsonCalibrationPoint["x"].toDouble(), jsonCalibrationPoint["y"].toDouble()));
    }

    this->isProjectionMapping = true;
}

void MappingWindow::updateCalibrationPoints() {

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

void MappingWindow::updateProjectionMapping() {
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


    if (calibrationPoints.size() == 0) {
        updateCalibrationPoints();
    }
    else {
        // Calibrate vertices based on the existing calibration points
        for (int i=0; i<vertices.size(); i++) {
            QVector3D* vertex = vertices[i];
            QVector2D* calibrationPoint = calibrationPoints[i];
            QVector2D* screenSpace = unNormalizeCoordinates(calibrationPoint->x(), calibrationPoint->y());

            applyVertexCalibration(vertex, screenSpace->x(), screenSpace->y());
        }

        updateMesh();
    }


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

void MappingWindow::updateMesh() {
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

void MappingWindow::updateProjectionMappingSettings() {
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

void MappingWindow::updateMVPmatrix() {
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

void MappingWindow::drawBillboard(float x, float y) {
    calibrationMatrix.translate(QVector3D(x, y, 0.));

    meshShader.setUniformValue("mvpMatrix", calibrationMatrix);
    calibrationVao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    calibrationVao.release();

    calibrationMatrix.translate(QVector3D(-x, -y, 0.));
}


QVector2D* MappingWindow::normalizeCoordinates(float x, float y) {
    x /= width();
    y /= height();

    x -= 0.5;
    y -= 0.5;

    x *= 2.0;
    y *= 2.0;

    x /= (float) height() / (float) width();

    return new QVector2D(x, y);
}

QVector2D* MappingWindow::unNormalizeCoordinates(float x, float y) {
    x *= (float) height() / (float) width();

    x /= 2.0;
    y /= 2.0;

    x += 0.5;
    y += 0.5;

    x *= width();
    y *= height();

    return new QVector2D(x, y);
}

void MappingWindow::applyVertexCalibration(QVector3D* vertex, float x, float y) {
    // Use the projected depth of the selected vertex to determine
    // the new position of the vertex in relation to its calibration point
    QVector3D transformedVertex = (*vertex).project(viewMatrix * modelMatrix, projectionMatrix, QRect(0, 0, width(), height()));
    (*vertex) = QVector3D(x, y, transformedVertex.z()).unproject(viewMatrix * modelMatrix, projectionMatrix, QRect(0, 0, width(), height()));
}

void MappingWindow::saveProjectionMappingConfig() {
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

void MappingWindow::resizeGL(int width, int height) {
    updateMVPmatrix();
}

void MappingWindow::mousePressEvent(QMouseEvent* event) {
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

void MappingWindow::mouseMoveEvent(QMouseEvent* event) {
    if (isCalibrating) {
        QPointF pos = event->localPos();
        float x = pos.rx();
        float y = height() - pos.ry();
        QVector2D* mousePos = normalizeCoordinates(x, y);

        if (selectedCalibrationPoint > -1) {
            QVector2D* calibrationPoint = calibrationPoints[selectedCalibrationPoint];
            calibrationPoint->setX(mousePos->x());
            calibrationPoint->setY(mousePos->y());


            applyVertexCalibration(vertices[selectedCalibrationPoint], x, y);

            updateMesh();
        }
    }
}

void MappingWindow::keyPressEvent(QKeyEvent* event) {
    Window::keyPressEvent(event);

    switch (event->key()) {
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
