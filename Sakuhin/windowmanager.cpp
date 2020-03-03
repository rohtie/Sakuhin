#include "windowmanager.h"
#include "window.h"
#include "visualswindow.h"
#include "mappingwindow.h"
#include "meshvisualswindow.h"
#include "timelinewindow.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {

}

void WindowManager::initialize(const QSurfaceFormat &format, Backend* backend, ShaderManager* shadermanager, SceneManager* scenemanager) {
    this->backend = backend;
    this->shadermanager = shadermanager;
    this->scenemanager = scenemanager;

    // Visuals mode
    // --------------------------
    Window* previewWindow = new VisualsWindow();

    previewWindow->initialize(backend, shadermanager, scenemanager, false, true);
    previewWindow->setFormat(format);
    previewWindow->resize(QSize(256, 256));
    previewWindow->setPosition(0, 400);
    previewWindow->setFlag(Qt::FramelessWindowHint);
    previewWindow->show();

    masterWindow = new MeshVisualsWindow();
    masterWindow->initialize(backend, shadermanager, scenemanager, true, false);
    masterWindow->setFormat(format);
    masterWindow->resize(QSize(512, 512));
    masterWindow->setPosition(256, 0);
    masterWindow->setFlag(Qt::FramelessWindowHint);
    masterWindow->show();

    windows.append(masterWindow);
    windows.append(previewWindow);
    // --------------------------


    // Projection mapping mode
    // --------------------------
    // masterWindow = new MappingWindow();
    // ((MappingWindow*) masterWindow)->setupMapping("data/config/sculpture.json");
    // masterWindow->initialize(backend, shadermanager, scenemanager, true, false);
    // masterWindow->setFormat(format);
    // masterWindow->resize(QSize(1280, 720));
    // masterWindow->setPosition(0, 0);
    // masterWindow->setFlag(Qt::FramelessWindowHint);
    // masterWindow->show();
    // windows.append(masterWindow);

    // Window* slaveWindow = new MappingWindow();
    // ((MappingWindow*) slaveWindow)->setupMapping("data/config/sculpture_back.json");
    // slaveWindow->initialize(backend, shadermanager, scenemanager, false, false);
    // slaveWindow->setFormat(format);
    // slaveWindow->resize(QSize(1280, 720));
    // slaveWindow->setPosition(0, 0);
    // slaveWindow->setFlag(Qt::FramelessWindowHint);
    // slaveWindow->show();
    // windows.append(slaveWindow);

    // Window* previewWindow = new VisualsWindow();
    // previewWindow->initialize(backend, shadermanager, scenemanager, false, true);
    // previewWindow->setFormat(format);
    // previewWindow->resize(QSize(512, 512));
    // previewWindow->setPosition(0, 0);
    // previewWindow->setFlag(Qt::FramelessWindowHint);
    // previewWindow->show();
    // windows.append(previewWindow);
    // --------------------------


    // Timeline mode
    // --------------------------
    // Window* previewWindow = new TimelineWindow();

    // previewWindow->initialize(backend, shadermanager, scenemanager, false, true);
    // previewWindow->setFormat(format);
    // previewWindow->resize(QSize(256, 256));
    // previewWindow->setPosition(256, 820);
    // previewWindow->setFlag(Qt::FramelessWindowHint);
    // previewWindow->show();

    // masterWindow = new TimelineWindow();
    // masterWindow->initialize(backend, shadermanager, scenemanager, true, false);
    // masterWindow->setFormat(format);
    // masterWindow->resize(QSize(960, 540));
    // masterWindow->setPosition(0, 0);
    // masterWindow->setFlag(Qt::FramelessWindowHint);
    // masterWindow->show();

    // windows.append(masterWindow);
    // windows.append(previewWindow);
    // --------------------------

    emit windowsChanged();
}
