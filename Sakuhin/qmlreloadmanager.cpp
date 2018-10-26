#include "qmlreloadmanager.h"

QmlReloadManager::QmlReloadManager(QQmlApplicationEngine* engine) {
    fileWatcher.addPath("../Sakuhin/main.qml");

    QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
                     this, &QmlReloadManager::onQmlChange);

    this->engine = engine;
}

void QmlReloadManager::onQmlChange(const QString &path) {
    engine->clearComponentCache();
    engine->load(path);
}
