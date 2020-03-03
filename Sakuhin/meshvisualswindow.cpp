#include "meshvisualswindow.h"

#include <QtMath>
#include <QOpenGLFramebufferObject>

#include "wavefrontobjectloader.h"
#include "shader.h"
#include "shadermanager.h"


MeshVisualsWindow::MeshVisualsWindow() {

}

void MeshVisualsWindow::initializeGL() {
    Window::initializeGL();

    postprocessingShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
    postprocessingShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/postprocessingShader.glsl");
    postprocessingShader.link();

    postprocessingShader.bind();
        postprocessingShader.setUniformValue("screenTexture", 0);
    postprocessingShader.release();

    loadObjects();
    updateViewProjectionmatrix();
}

void MeshVisualsWindow::loadObjects() {
    if (hasLoadedObjects) {
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create initial mesh shader
    meshShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/meshVertex.glsl");
    meshShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/meshShader.glsl");
    meshShader.link();

    meshShader.bind();
        meshShader.setUniformValue("mainTexture", 0);
    meshShader.release();

    // Load textures
    meshTexture = new QOpenGLTexture(QImage("data/objects/test2.png").mirrored());
    meshTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    meshTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Load objects
    WavefrontObjectLoader::loadMesh("data/objects/test2.obj", meshVertices, meshUVs);

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

    hasLoadedObjects = true;
}

void MeshVisualsWindow::updateViewProjectionmatrix() {
    // View matrix
    QVector3D camera(0, 0, 10);
    QVector3D target(0, 0, 0);
    QVector3D upVector(0, 1, 0);

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(camera, target, upVector);

    QMatrix4x4 cameraMatrix = viewMatrix.inverted();
    cameraForward = (cameraMatrix * QVector4D(0., 0., 1., 0.)).toVector3D();
    cameraRight = (cameraMatrix * QVector4D(1., 0., 0., 0.)).toVector3D();
    cameraUp = (cameraMatrix * QVector4D(0., 1., 0., 0.)).toVector3D();

    // Projection matrix
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fieldOfView, (float) width() / (float) height(), cameraNear, cameraFar);

    // VP matrix that is sent to the vertex shader
    viewProjectionMatrix.setToIdentity();
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void MeshVisualsWindow::renderScreen(Shader* shader) {
    if (isMaster) {
        render(shader);
    }
    else if (isPreview && !shadermanager->previewIsMain()) {
        render(shader);
    }

    shader->enableDepth();
    shader->setResolution(width(), height());
    QOpenGLFramebufferObject* fbo = shader->currentFbo();

    fbo->bind();
        if (hasLoadedObjects) {
            meshShader.bind();
                glClear(GL_DEPTH_BUFFER_BIT);

                modelMatrix.setToIdentity();
                modelMatrix.translate( qSin(time.elapsed() * 0.001) * 3.0, 0, 0);
                modelMatrix.rotate(time.elapsed() * 0.05, QVector3D(1., 0.2, 0.7));
                meshShader.setUniformValue("mvpMatrix", viewProjectionMatrix * modelMatrix);

                meshTexture->bind(0);

                meshVao.bind();
                    glDrawArrays(GL_TRIANGLES, 0, meshVertices.size());
                meshVao.release();

            meshShader.release();
        }

    fbo->release();

    postprocessingShader.bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        postprocessingShader.setUniformValue("resolution", width(), height());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shader->currentFrame());

        drawRectangle();
    postprocessingShader.release();
}

void MeshVisualsWindow::resizeGL(int width, int height) {
    updateViewProjectionmatrix();
}
