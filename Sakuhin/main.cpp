#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.h"
#include "window.h"

#include "shadermanager.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("sakuhin.backend", 1, 0, "BackEnd");
    qmlRegisterType<ShaderManager>("sakuhin.shadermanager", 1, 0, "ShaderManager");

    QQmlApplicationEngine engine("qrc:/main.qml");

    QObject* qmlRoot = engine.rootObjects()[0];

    BackEnd* backend = qmlRoot->findChild<BackEnd*>();
    ShaderManager* shadermanager = qmlRoot->findChild<ShaderManager*>();
    shadermanager->sessionID = backend->getSessionID();


    backend->shadermanager = shadermanager;

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);

    // Disable vsync for more accurate performance measures
    // and getting rid of screen tearing
    format.setSwapInterval(0);

    Window previewWindow(backend, shadermanager, true);
    previewWindow.setFormat(format);
    previewWindow.resize(QSize(256, 256));
    previewWindow.setFlag(Qt::FramelessWindowHint);
    previewWindow.show();

    Window mainWindow(backend, shadermanager, false);
    mainWindow.setFormat(format);
    mainWindow.resize(QSize(800, 600));
    mainWindow.show();

    return app.exec();
}
