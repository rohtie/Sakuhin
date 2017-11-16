#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QObject>
#include <QAudioDeviceInfo>

class AudioDevice : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)

    public:
        explicit AudioDevice(QAudioDeviceInfo* deviceInfo);

    signals:
        void nameChanged();

    private:
        QString name;
        QAudioDeviceInfo* deviceInfo;
};

#endif // AUDIODEVICE_H
