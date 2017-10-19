#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "backend.h"
#include "window.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("shikaku.backend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);

    Window window;
    window.setFormat(format);
    window.resize(QSize(800, 600));
    window.show();

    return app.exec();
}
