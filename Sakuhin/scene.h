#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "shader.h"

class Scene : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
    Q_PROPERTY(int length MEMBER length NOTIFY lengthChanged)
    Q_PROPERTY(int originalLength MEMBER originalLength NOTIFY originalLengthChanged)

    public:
        explicit Scene(QObject *parent = nullptr);
        Scene(const QString name, int shaderIndex);

        int shaderIndex = 0;
        QString name;
        int length = 20000;
        int originalLength = 20000;

    signals:
        void nameChanged();
        void lengthChanged();
        void originalLengthChanged();

    public slots:
};

#endif // SCENE_H
