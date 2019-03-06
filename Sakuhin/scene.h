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
        Scene(const QString name);

        Shader* shader;
        QString name;
        int length = 500;
        int originalLength = 500;


    signals:
        void nameChanged();
        void lengthChanged();
        void originalLengthChanged();

    public slots:
};

#endif // SCENE_H
