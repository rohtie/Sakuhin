#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QFileSystemWatcher>
#include <QDateTime>

#include "backend.h"

class QOpenGLShaderProgram;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    public:
        explicit Window(BackEnd *backend);

        QString buildShader();
        void recompileShader();

        void initializeGL();
        void paintGL();

    public slots:
        void onSessionFileChange(const QString &path);

    private:
    	BackEnd *backend;

        QString sessionPath;
        QFileSystemWatcher fileWatcher;
        QDateTime lastSessionModification;
        QByteArray sessionContents;

        QOpenGLBuffer vbo;
        QOpenGLVertexArrayObject vao;
        QOpenGLShaderProgram shader;
};

#endif // WINDOW_H
