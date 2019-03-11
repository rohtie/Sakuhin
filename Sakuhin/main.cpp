#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.h"
#include "window.h"

#include "shadermanager.h"
#include "audiomanager.h"
#include "windowmanager.h"
#include "scenemanager.h"

#include "qmlreloadmanager.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("sakuhin.backend", 1, 0, "BackEnd");
    qmlRegisterType<ShaderManager>("sakuhin.shadermanager", 1, 0, "ShaderManager");
    qmlRegisterType<AudioManager>("sakuhin.audiomanager", 1, 0, "AudioManager");
    qmlRegisterType<WindowManager>("sakuhin.windowmanager", 1, 0, "WindowManager");
    qmlRegisterType<SceneManager>("sakuhin.scenemanager", 1, 0, "SceneManager");

    QQmlApplicationEngine engine("qrc:/main.qml");

    QObject* qmlRoot = engine.rootObjects()[0];

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setDepthBufferSize(24);

    #ifdef QT_DEBUG
    // Allow livereload when editing qml files
    QmlReloadManager reloadManager(&engine);
    format.setOption(QSurfaceFormat::DebugContext);
    #endif

    // Disable vsync for more accurate performance measures
    format.setSwapInterval(0);


    BackEnd* backend = qmlRoot->findChild<BackEnd*>();
    AudioManager* audiomanager = qmlRoot->findChild<AudioManager*>();
    ShaderManager* shadermanager = qmlRoot->findChild<ShaderManager*>();
    WindowManager* windowmanager = qmlRoot->findChild<WindowManager*>();
    SceneManager* sceneManager = qmlRoot->findChild<SceneManager*>();

    shadermanager->sessionID = backend->getSessionID();
    backend->shadermanager = shadermanager;

    audiomanager->initialize();
    shadermanager->initialize(format);
    windowmanager->initialize(format, backend, shadermanager, sceneManager);
    sceneManager->initialize(shadermanager);

    return app.exec();
}
