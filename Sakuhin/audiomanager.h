#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>

class AudioManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> audioDevices MEMBER audioDevices NOTIFY audioDevicesChanged)

    public:
        explicit AudioManager(QObject *parent = nullptr);
        void initialize();

    signals:
        void audioDevicesChanged();

    private:
        QList<QObject*> audioDevices;
};

#endif // AUDIOMANAGER_H