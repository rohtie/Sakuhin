#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "backend.h"
#include "window.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("sakuhin.backend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    BackEnd* backend = engine.rootObjects()[0]->findChild<BackEnd *>();

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);

    // Disable vsync for more accurate performance measures
    // and getting rid of screen tearing
    format.setSwapInterval(0);

    Window previewWindow(backend);
    previewWindow.setFormat(format);
    previewWindow.resize(QSize(256, 256));
    previewWindow.setFlag(Qt::FramelessWindowHint);
    previewWindow.show();

    Window mainWindow(backend);
    mainWindow.setFormat(format);
    mainWindow.resize(QSize(800, 600));
    mainWindow.show();

    return app.exec();
}
