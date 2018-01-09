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

    Q_PROPERTY(bool isMaster MEMBER isMaster NOTIFY isMasterChanged)
    Q_PROPERTY(bool isPreview MEMBER isPreview NOTIFY isPreviewChanged)
    Q_PROPERTY(bool isProjectionMapping MEMBER isProjectionMapping NOTIFY isProjectionMappingChanged)
    Q_PROPERTY(bool isVertical MEMBER isVertical NOTIFY isVerticalChanged)
    Q_PROPERTY(float distanceFromObject MEMBER distanceFromObject NOTIFY distanceFromObjectChanged)
    Q_PROPERTY(float projectorHeight MEMBER projectorHeight NOTIFY projectorHeightChanged)
    Q_PROPERTY(float fieldOfView MEMBER fieldOfView NOTIFY fieldOfViewChanged)

    public:
        explicit Window();

        void setupMapping(float distanceFromObject, float projectorHeight, float fieldOfView, bool isVertical, const QString &modelPath);
        void initialize(BackEnd* backend, ShaderManager* shadermanager, bool isMaster, bool isPreview);

        void initializeGL();

        void drawRectangle();
        void render(Shader* shader);
        void renderScreen(Shader* shader);
        void paintGL();

        void keyPressEvent(QKeyEvent* event);
        void updatePerformanceInformation();
        void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

        void resizeGL(int width, int height);

    public slots:
        void updateProjectionMapping();
        void updateMVPmatrix();

    signals:
        void isMasterChanged();
        void isPreviewChanged();
        void isProjectionMappingChanged();
        void isVerticalChanged();
        void distanceFromObjectChanged();
        void projectorHeightChanged();
        void fieldOfViewChanged();

    private:
        bool isMaster = false;
        bool isPreview = false;
        bool isProjectionMapping = false;
        bool hasLoadedProjectionObject = false;

        float distanceFromObject = -400.0;
        float projectorHeight = 125.;
        float fieldOfView = 45.2397;
        bool isVertical = false;
        QString modelPath;

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
