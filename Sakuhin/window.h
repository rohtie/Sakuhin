#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QFileSystemWatcher>
#include <QDateTime>
#include <QElapsedTimer>
#include <QOpenGLTexture>
#include <QVector>
#include <QOpenGLDebugMessage>
#include <QMatrix4x4>

#include "backend.h"
#include "shadermanager.h"

class QOpenGLShaderProgram;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    public:
        explicit Window(BackEnd* backend, ShaderManager* shadermanager, bool isPreview, bool isProjectionMapping);

        void initializeGL();

        void drawRectangle();
        void render(Shader* shader);
        void renderScreen(Shader* shader);
        void paintGL();

        void keyPressEvent(QKeyEvent* event);
        void updatePerformanceInformation();
        void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

    signals:
        void shaderRecompiled();

    private:
        bool isPreview = false;
        bool isProjectionMapping = false;

    	BackEnd* backend;
        ShaderManager* shadermanager;

        QOpenGLShaderProgram screenShader;
        QOpenGLVertexArrayObject rectangleVao;
        QOpenGLBuffer rectangleVertexBuffer;

        QOpenGLShaderProgram meshShader;
        QOpenGLVertexArrayObject meshVao;
        QOpenGLBuffer meshVertexBuffer;
        QOpenGLBuffer meshUVbuffer;
        QVector<GLfloat> meshVertices;
        QVector<GLfloat> meshUVs;
        QMatrix4x4 modelMatrix;
        QMatrix4x4 viewMatrix;
        QMatrix4x4 projectionMatrix;
        QMatrix4x4 mvpMatrix;

        QElapsedTimer time;
        int frameCounter;
        qint64 lastTime = 0;
        QMargins oldMargins;

        QRect oldGeometry;
};

#endif // WINDOW_H
