#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>

#include "scene.h"

class SceneManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> scenes MEMBER scenes NOTIFY scenesChanged)

    public:
        explicit SceneManager(QObject *parent = nullptr);
        void initialize();
        Q_INVOKABLE void swap(int indexA, int indexB);

    signals:
        void scenesChanged();

    public slots:

    private:
        QList<QObject*> scenes;
};

#endif // SCENEMANAGER_H
