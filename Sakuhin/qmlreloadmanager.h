#ifndef QMLRELOADMANAGER_H
#define QMLRELOADMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>
#include <QDebug>

class QmlReloadManager : public QObject {
    Q_OBJECT

    public:
        QmlReloadManager(QQmlApplicationEngine* engine);

    public slots:
        void onQmlChange(const QString &path);

    private:
        QFileSystemWatcher fileWatcher;
        QQmlApplicationEngine* engine;
};

#endif // QMLRELOADMANAGER_H
