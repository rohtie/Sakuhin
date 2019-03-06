#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include "window.h"
#include "visualswindow.h"
#include "timelinewindow.h"

class WindowManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> windows MEMBER windows NOTIFY windowsChanged)

    public:
        explicit WindowManager(QObject *parent = nullptr);
        void initialize(const QSurfaceFormat &format, BackEnd* backend, ShaderManager* shadermanager, SceneManager* scenemanager);
        void createWindow();

    signals:
        void windowsChanged();

    public slots:

    private:
        BackEnd* backend;
        ShaderManager* shadermanager;
        SceneManager* scenemanager;

        Window* masterWindow;
        QList<QObject*> windows;
};

#endif // WINDOWMANAGER_H
