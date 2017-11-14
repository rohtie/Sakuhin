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

#include "backend.h"
#include "shadermanager.h"

class QOpenGLShaderProgram;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    public:
        explicit Window(BackEnd* backend, ShaderManager* shadermanager, bool isPreview);

        void initializeGL();

        void drawRectangle();
        void render(Shader* shader);
        void renderScreen(Shader* shader);
        void updatePerformanceInformation();
        void paintGL();

        void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

    signals:
        void shaderRecompiled();

    private:
        bool isPreview = false;

    	BackEnd* backend;
        ShaderManager* shadermanager;

        QOpenGLShaderProgram screenShader;
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;

        QElapsedTimer time;
        int frameCounter;
        qint64 lastTime = 0;
};

#endif // WINDOW_H
