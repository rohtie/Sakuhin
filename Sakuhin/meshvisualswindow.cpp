#include "meshvisualswindow.h"

#include <QtMath>
#include <QDir>
#include <QOpenGLFramebufferObject>

#include "wavefrontobjectloader.h"
#include "shader.h"
#include "shadermanager.h"


MeshVisualsWindow::MeshVisualsWindow() {

}

void MeshVisualsWindow::initializeGL() {
    Window::initializeGL();

    // Watch postprocessing shader and vertex shader for changes
    QFile postProcessingShaderFile(":/postprocessingShader.glsl");
    if (!postProcessingShaderFile.copy(shadermanager->sessionPath + "/postprocessing.glsl")) {
        return;
    }
    postProcessingShaderFile.close();

    QFile meshVertexShaderFile(":/liveMeshVertex.glsl");
    if (!meshVertexShaderFile.copy(shadermanager->sessionPath + "/meshVertex.glsl")) {
        return;
    }
    meshVertexShaderFile.close();

    fileWatcher.addPath(shadermanager->sessionPath + "/postprocessing.glsl");
    fileWatcher.addPath(shadermanager->sessionPath + "/meshVertex.glsl");

    QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
                     this, &MeshVisualsWindow::onShaderFileChange);


    // Create postprocessing shader
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

    // Load objects
    Mesh mesh;

    QDir textureDirectory("data/objects/texture/");
    QStringList textureNames = textureDirectory.entryList(QDir::Files);

    QDir meshDirectory("data/objects/mesh/");
    QStringList meshNames = meshDirectory.entryList(QDir::Files);


    QVector<GLfloat> meshVertices;
    QVector<GLfloat> meshUVs;

    for (int i=0; i<textureNames.length(); i++) {
        meshVertices.clear();
        meshUVs.clear();

        // Load texture
        mesh.texture = new QOpenGLTexture(QImage("data/objects/texture/" + textureNames[i]).mirrored());
        mesh.texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        mesh.texture->setMagnificationFilter(QOpenGLTexture::Linear);

        // Load mesh
        WavefrontObjectLoader::loadMesh("data/objects/mesh/" + meshNames[i], meshVertices, meshUVs);
        mesh.vertexCount = meshVertices.length();

        mesh.vao = new QOpenGLVertexArrayObject();
        mesh.vao->create();
        mesh.vao->bind();
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
        mesh.vao->release();

        meshVertexBuffer.destroy();
        meshUVbuffer.destroy();

        meshes.append(mesh);
    }

    hasLoadedObjects = true;
}

void MeshVisualsWindow::RecompileShader(QOpenGLShaderProgram &program, const QString &path, bool isVertexShader) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray code = file.readAll();
    file.close();

    if (code.size() > 0) {
        QOpenGLShader* vertexShader = program.shaders()[0];
        QOpenGLShader* fragmentShader = program.shaders()[1];

        program.removeShader(vertexShader);
        program.removeShader(fragmentShader);

        if (isVertexShader) {
            if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, code)) {
                program.addShader(vertexShader);
            }

            program.addShader(fragmentShader);

            program.link();
        }
        else {
            program.addShader(vertexShader);

            if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, code)) {
                program.addShader(fragmentShader);
            }

            program.link();
        }
    }
}

void MeshVisualsWindow::onShaderFileChange(const QString &path) {

    QFileInfo fileInfo(path);

    if (path == shadermanager->sessionPath + "/postprocessing.glsl") {
        if (fileInfo.lastModified() > lastPostprocessingModification) {
            lastPostprocessingModification = fileInfo.lastModified();
            RecompileShader(postprocessingShader, path, false);
        }
    }
    else if (path == shadermanager->sessionPath + "/meshVertex.glsl") {
        if (fileInfo.lastModified() > lastMeshVertexModification) {
            lastMeshVertexModification = fileInfo.lastModified();
            RecompileShader(meshShader, path, true);
        }
    }

    // On some systems, files are replaced by an entirely new file when
    // modified. thus we need to add it to the file watcher again.
    if (!fileWatcher.files().contains(path) && fileInfo.exists()) {
        fileWatcher.addPath(path);
    }
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

                int numberOfDrawnObjects = 0;
                while (numberOfDrawnObjects < targetNumberOfVisibleObjects) {
                    int currentMeshIndex = (meshIndex + numberOfDrawnObjects) % meshes.length();
                    Mesh mesh = meshes[currentMeshIndex];

                    meshShader.setUniformValue("mvpMatrix", viewProjectionMatrix);
                    meshShader.setUniformValue("time", (GLfloat) (time.elapsed() / 1000.0f));

                    mesh.texture->bind(0);

                    mesh.vao->bind();
                        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
                    mesh.vao->release();

                    numberOfDrawnObjects++;
                }

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
