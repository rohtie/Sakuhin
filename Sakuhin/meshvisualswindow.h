#ifndef MESHVISUALSWINDOW_H
#define MESHVISUALSWINDOW_H

#include <QOpenGLTexture>

#include "window.h"

class MeshVisualsWindow : public Window {
    public:
        MeshVisualsWindow();

        void initializeGL();

        void loadObjects();
        void updateViewProjectionmatrix();
        void renderScreen(Shader* shader);

        void resizeGL(int width, int height);


    public slots:

    signals:

    protected:
        QOpenGLShaderProgram postprocessingShader;

        QOpenGLShaderProgram meshShader;

        QOpenGLVertexArrayObject meshVao;
        QOpenGLBuffer meshVertexBuffer;
        QOpenGLBuffer meshUVbuffer;

        QOpenGLTexture* meshTexture;

        QVector<GLfloat> meshVertices;
        QVector<GLfloat> meshUVs;

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
};

#endif // MESHVISUALSWINDOW_H
