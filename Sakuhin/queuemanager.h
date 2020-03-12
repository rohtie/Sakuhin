#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

class ShaderManager;
class Scene;

#include <QObject>
#include <QDateTime>
#include <QJsonArray>

class QueueManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> queue MEMBER queue NOTIFY queueChanged)
    Q_PROPERTY(int sceneIndex MEMBER sceneIndex NOTIFY sceneIndexChanged)

    public:
        explicit QueueManager(QObject *parent = nullptr);
        void initialize(ShaderManager* shadermanager, const QJsonArray &jsonScenes);
        Q_INVOKABLE void swap(int indexA, int indexB);
        Q_INVOKABLE void newScene();
        Q_INVOKABLE void selectScene(int index);
        Q_INVOKABLE void previousScene();
        Q_INVOKABLE void nextScene();

        int sceneIndex = 0;

        QList<QObject*> queue;

    signals:
        void queueChanged();
        void sceneIndexChanged();

    public slots:
        void onShaderIndexChange();

    private:
        ShaderManager* shadermanager;

};

#endif // QUEUEMANAGER_H
