#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class QOpenGLShaderProgram;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    public:
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();

    private:
        QOpenGLBuffer vbo;
        QOpenGLVertexArrayObject vao;
        QOpenGLShaderProgram shader;
};

#endif // WINDOW_H
