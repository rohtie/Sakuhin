#ifndef WINDOW_H
#define WINDOW_H

class Backend;
class SceneManager;
class ShaderManager;
class QueueManager;
class Shader;

#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QDateTime>
#include <QElapsedTimer>
#include <QOpenGLDebugMessage>

// #include "videorecorder.h"

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions {
    Q_OBJECT

    Q_PROPERTY(bool isMaster MEMBER isMaster NOTIFY isMasterChanged)
    Q_PROPERTY(bool isPreview MEMBER isPreview NOTIFY isPreviewChanged)


    public:
        explicit Window();

        void initialize(Backend* backend, ShaderManager* shadermanager, SceneManager* scenemanager, QueueManager* queuemanager, bool isMaster, bool isPreview);

        virtual void initializeGL();

        void drawRectangle();
        void render(Shader* shader);
        virtual void renderScreen(Shader* shader);
        virtual void processTime();
        virtual void setupRecordingTime();
        virtual void handleRecordingTime();
        void paintGL();

        virtual void keyPressEvent(QKeyEvent* event);

        void updatePerformanceInformation();
        void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);


    public slots:

    signals:
        void isMasterChanged();
        void isPreviewChanged();

    protected:
        bool isMaster = false;
        bool isPreview = false;

        bool isRecording = false;
        bool isTakingScreenshot = false;
        int screenshotNumber = 0;

        // VideoRecorder videoRecorder;
        uint8_t* recordingFrameData;

        int recordingBufferSize;
        float recordingFramerate = 60.0;
        float recordingFrame = 0.0;
        float recordingStartTime = 0.0;

        Backend* backend;
        ShaderManager* shadermanager;
        SceneManager* scenemanager;
        QueueManager* queuemanager;

        QOpenGLShaderProgram screenShader;
        QOpenGLVertexArrayObject rectangleVao;
        QOpenGLBuffer rectangleVertexBuffer;

        QElapsedTimer time;
        int frameCounter;
        qint64 currentTime = 0;
        qint64 performanceTime = 0;
        qint64 lastTime = 0;

        QMargins oldMargins;
        QRect oldGeometry;
};

#endif // WINDOW_H
