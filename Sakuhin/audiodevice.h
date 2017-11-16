#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioInput>

class AudioDevice : public QIODevice {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)

    public:
        explicit AudioDevice(const QAudioDeviceInfo &deviceInfo);

        void start();
        void stop();

        qint64 readData(char *data, qint64 maxlen) override;
        qint64 writeData(const char *data, qint64 len) override;

    signals:
        void nameChanged();

    private:
        QString name;
        QAudioDeviceInfo deviceInfo;
        QAudioInput* audioInput;
};

#endif // AUDIODEVICE_H
