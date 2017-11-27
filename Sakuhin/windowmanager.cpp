#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {

}

void WindowManager::initialize(const QSurfaceFormat &format, BackEnd* backend, ShaderManager* shadermanager) {
    this->backend = backend;
    this->shadermanager = shadermanager;

    masterWindow = new Window();
    masterWindow->initialize(backend, shadermanager, true, false, true);
    masterWindow->setFormat(format);
    masterWindow->resize(QSize(256, 256));
    masterWindow->show();

    Window* previewWindow = new Window();
    previewWindow->initialize(backend, shadermanager, false, true, false);
    previewWindow->setFormat(format);
    previewWindow->resize(QSize(256, 256));
    previewWindow->setFlag(Qt::FramelessWindowHint);
    previewWindow->show();

    Window* slaveWindow = new Window();
    slaveWindow->initialize(backend, shadermanager, false, false, true);
    slaveWindow->setFormat(format);
    slaveWindow->resize(QSize(256, 256));
    slaveWindow->setFlag(Qt::FramelessWindowHint);
    slaveWindow->show();

    windows.append(masterWindow);
    windows.append(previewWindow);
    windows.append(slaveWindow);

    emit windowsChanged();
}
