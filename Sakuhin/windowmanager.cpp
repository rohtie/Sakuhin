#include "windowmanager.h"
#include "window.h"
#include "visualswindow.h"
#include "timelinewindow.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {

}

void WindowManager::initialize(const QSurfaceFormat &format, Backend* backend, ShaderManager* shadermanager, SceneManager* scenemanager) {
    this->backend = backend;
    this->shadermanager = shadermanager;
    this->scenemanager = scenemanager;

    Window* previewWindow = new TimelineWindow();

    previewWindow->initialize(backend, shadermanager, scenemanager, false, true);
    previewWindow->setFormat(format);
    previewWindow->resize(QSize(256, 256));
    previewWindow->setPosition(0, 400);
    previewWindow->setFlag(Qt::FramelessWindowHint);
    previewWindow->show();

    masterWindow = new TimelineWindow();
    masterWindow->initialize(backend, shadermanager, scenemanager, true, false);
    masterWindow->setFormat(format);
    masterWindow->resize(QSize(512, 512));
    masterWindow->setPosition(256, 0);
    masterWindow->setFlag(Qt::FramelessWindowHint);
    masterWindow->show();

    windows.append(masterWindow);
    windows.append(previewWindow);

    emit windowsChanged();
}
