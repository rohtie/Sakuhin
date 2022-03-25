#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

class Backend;
class ShaderManager;
class SceneManager;
class QueueManager;
class Window;

#include <QObject>
#include <QSurfaceFormat>

class WindowManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> windows MEMBER windows NOTIFY windowsChanged)

    public:
        explicit WindowManager(QObject *parent = nullptr);
        void initialize(const QSurfaceFormat &format, Backend* backend, ShaderManager* shadermanager, SceneManager* scenemanager, QueueManager* queuemanager);
        void createWindow();

    signals:
        void windowsChanged();

    public slots:

    private:
        Backend* backend;
        ShaderManager* shadermanager;
        SceneManager* scenemanager;
        QueueManager* queuemanager;

        Window* masterWindow;
        QList<QObject*> windows;
};

#endif // WINDOWMANAGER_H
