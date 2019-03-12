#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.h"
#include "window.h"

#include "sessionmanager.h"
#include "shadermanager.h"
#include "audiomanager.h"
#include "windowmanager.h"
#include "scenemanager.h"

#include "qmlreloadmanager.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Backend>("sakuhin.backend", 1, 0, "Backend");
    qmlRegisterType<SessionManager>("sakuhin.sessionmanager", 1, 0, "SessionManager");
    qmlRegisterType<ShaderManager>("sakuhin.shadermanager", 1, 0, "ShaderManager");
    qmlRegisterType<AudioManager>("sakuhin.audiomanager", 1, 0, "AudioManager");
    qmlRegisterType<WindowManager>("sakuhin.windowmanager", 1, 0, "WindowManager");
    qmlRegisterType<SceneManager>("sakuhin.scenemanager", 1, 0, "SceneManager");

    QQmlApplicationEngine engine("qrc:/main.qml");

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setDepthBufferSize(24);

    #ifdef QT_DEBUG
    // Allow livereload when editing qml files
    QmlReloadManager reloadManager(&engine);

    // OpenGL debug context
    format.setOption(QSurfaceFormat::DebugContext);
    #endif

    // Disable vsync for more accurate performance measures
    format.setSwapInterval(0);

    QObject* qmlRoot = engine.rootObjects()[0];
    Backend* backend = qmlRoot->findChild<Backend*>();
    backend->initialize(format, qmlRoot);

    return app.exec();
}
