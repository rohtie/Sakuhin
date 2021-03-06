#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QOpenGLTexture>

#include <fftw3.h>


class AudioDevice : public QIODevice {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)

    public:
        explicit AudioDevice(const QAudioDeviceInfo &deviceInfo);

        void start();
        void stop();

        qint64 readData(char* data, qint64 maxlen) override;
        qint64 writeData(const char* data, qint64 len) override;

        QOpenGLTexture* spectrumTexture();
        QString name;

    signals:
        void nameChanged();

    private:
        QAudioDeviceInfo deviceInfo;
        QAudioInput* audioInput;
        QOpenGLTexture* texture;

        const int spectrumWidth = 512;
        QVector<double> blackman;
};

#endif // AUDIODEVICE_H
