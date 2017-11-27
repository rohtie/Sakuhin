#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.h"
#include "window.h"

#include "shadermanager.h"
#include "audiomanager.h"
#include "windowmanager.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("sakuhin.backend", 1, 0, "BackEnd");
    qmlRegisterType<ShaderManager>("sakuhin.shadermanager", 1, 0, "ShaderManager");
    qmlRegisterType<AudioManager>("sakuhin.audiomanager", 1, 0, "AudioManager");
    qmlRegisterType<WindowManager>("sakuhin.windowmanager", 1, 0, "WindowManager");

    QQmlApplicationEngine engine("qrc:/main.qml");

    QObject* qmlRoot = engine.rootObjects()[0];

    BackEnd* backend = qmlRoot->findChild<BackEnd*>();
    ShaderManager* shadermanager = qmlRoot->findChild<ShaderManager*>();
    shadermanager->sessionID = backend->getSessionID();
    backend->shadermanager = shadermanager;

    AudioManager* audiomanager = qmlRoot->findChild<AudioManager*>();
    audiomanager->initialize();

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setDepthBufferSize(24);

    #ifdef QT_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
    #endif

    // Disable vsync for more accurate performance measures
    // and getting rid of screen tearing
    format.setSwapInterval(0);

    WindowManager* windowmanager = qmlRoot->findChild<WindowManager*>();
    shadermanager->initialize(format);
    windowmanager->initialize(format, backend, shadermanager);

    return app.exec();
}
