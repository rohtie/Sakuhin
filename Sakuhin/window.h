#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "backend.h"

class QOpenGLShaderProgram;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    public:
        explicit Window(BackEnd *backend);

        QString buildShader();

        void initializeGL();
        void paintGL();

    private:
    	BackEnd *backend;

        QOpenGLBuffer vbo;
        QOpenGLVertexArrayObject vao;
        QOpenGLShaderProgram shader;
};

#endif // WINDOW_H
