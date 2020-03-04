#ifndef MESHVISUALSWINDOW_H
#define MESHVISUALSWINDOW_H

#include <QOpenGLTexture>
#include <QFileSystemWatcher>
#include <QDateTime>
#include <QTimer>

#include "window.h"

class MeshVisualsWindow : public Window {

    struct Mesh {
        QOpenGLVertexArrayObject* vao;
        QOpenGLTexture* texture;
        int vertexCount;
    };

    public:
        MeshVisualsWindow();

        void initializeGL();

        void loadObjects();
        void RecompileShader(QOpenGLShaderProgram &program, const QString &path, bool isVertexShader);
        void updateViewProjectionmatrix();
        void renderScreen(Shader* shader);

        void resizeGL(int width, int height);


    public slots:
        void onShaderFileChange(const QString &path);
        void changeMeshes();

    signals:

    protected:
        QFileSystemWatcher fileWatcher;

        QOpenGLShaderProgram postprocessingShader;
        QOpenGLShaderProgram meshShader;

        QVector<Mesh> meshes;

        QOpenGLBuffer meshVertexBuffer;
        QOpenGLBuffer meshUVbuffer;

        QMatrix4x4 viewMatrix;
        QMatrix4x4 projectionMatrix;
        QMatrix4x4 viewProjectionMatrix;
        QMatrix4x4 modelMatrix;

        QVector3D cameraForward;
        QVector3D cameraRight;
        QVector3D cameraUp;

        float fieldOfView = 45.2397;
        float cameraNear = 1.0;
        float cameraFar = 1000.0;

        bool hasLoadedObjects = false;

        QTimer meshTimer;
        int meshIndex = 0;
        int targetNumberOfVisibleObjects = 2;
        int meshChangeInterval = 60000;

        QDateTime lastPostprocessingModification;
        QDateTime lastMeshVertexModification;
};

#endif // MESHVISUALSWINDOW_H
