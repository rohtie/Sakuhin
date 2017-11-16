#include "audiodevice.h"
#include <QDebug>
#include "qendian.h"

AudioDevice::AudioDevice(const QAudioDeviceInfo &deviceInfo) {
    name = deviceInfo.deviceName();
    this->deviceInfo = deviceInfo;

    emit nameChanged();
}

void AudioDevice::start() {
    audioInput = new QAudioInput(deviceInfo, deviceInfo.preferredFormat());
    open(QIODevice::WriteOnly);
    audioInput->start(this);
}

void AudioDevice::stop() {
    close();
}

qint64 AudioDevice::readData(char *data, qint64 maxlen) {
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 AudioDevice::writeData(const char *data, qint64 len) {
    QAudioFormat format = audioInput->format();

    Q_ASSERT(format.sampleSize() % 8 == 0);
    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes = format.channelCount() * channelBytes;
    Q_ASSERT(len % sampleBytes == 0);
    const int numSamples = len / sampleBytes;

    quint32 maxValue = 0;
    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < format.channelCount(); ++j) {
            quint32 value = 0;

            if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                value = *reinterpret_cast<const quint8*>(ptr);
            } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                value = qAbs(*reinterpret_cast<const qint8*>(ptr));
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint16>(ptr);
                else
                    value = qFromBigEndian<quint16>(ptr);
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qAbs(qFromLittleEndian<qint16>(ptr));
                else
                    value = qAbs(qFromBigEndian<qint16>(ptr));
            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::UnSignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint32>(ptr);
                else
                    value = qFromBigEndian<quint32>(ptr);
            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::SignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qAbs(qFromLittleEndian<qint32>(ptr));
                else
                    value = qAbs(qFromBigEndian<qint32>(ptr));
            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::Float) {
                value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
            }

            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }

    qDebug() << maxValue;

    return len;
}
