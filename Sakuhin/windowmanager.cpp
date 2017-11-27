#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {

}

void WindowManager::initialize(const QSurfaceFormat &format, BackEnd* backend, ShaderManager* shadermanager) {
    this->backend = backend;
    this->shadermanager = shadermanager;

    masterWindow = new Window();
    masterWindow->initialize(backend, shadermanager, true, false, false);
    masterWindow->setFormat(format);
    masterWindow->resize(QSize(720 * 0.5, 1280 * 0.5));
    masterWindow->show();

    Window* previewWindow = new Window();
    previewWindow->initialize(backend, shadermanager, false, true, false);
    previewWindow->setFormat(format);
    previewWindow->resize(QSize(256, 256));
    previewWindow->setFlag(Qt::FramelessWindowHint);
    previewWindow->show();

    windows.append(masterWindow);
    windows.append(previewWindow);

    emit windowsChanged();
}
