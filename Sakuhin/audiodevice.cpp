#include "audiodevice.h"
#include <QDebug>
#include <QtMath>
#include "qendian.h"

AudioDevice::AudioDevice(const QAudioDeviceInfo &deviceInfo) {
    name = deviceInfo.deviceName();
    this->deviceInfo = deviceInfo;

    emit nameChanged();
}

void AudioDevice::start() {
    audioInput = new QAudioInput(deviceInfo, deviceInfo.preferredFormat());
    open(QIODevice::WriteOnly);

    QAudioFormat format = audioInput->format();

    #ifdef QT_DEBUG
    qDebug() << "Audio input samplesize:" << format.sampleSize() << "bit";
    #endif

    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setSize(spectrumWidth, 1);
    texture->setFormat(QOpenGLTexture::R32F);
    texture->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Float32);
    texture->create();

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

qint64 AudioDevice::writeData(const char* data, qint64 len) {
    // TODO: Smooth the fft spectrum
    // TODO: Apply blackman window for better distribution of frequencies

    QAudioFormat format = audioInput->format();

    const int channelBytes = format.sampleSize() / 8;
    const int channelCount = format.channelCount();
    const int sampleBytes = channelCount * channelBytes;
    const int numSamples = len / sampleBytes;

    const int targetSamples = spectrumWidth * 2;
    const int fftwSamples = targetSamples / 2 + 1;
    const double fftwScale = 0.0125 * fftwSamples;

    // Because the data read from the audio input
    // can vary a lot in size we must zero pad the
    // sample passed to fft to ensure correct output
    // size for the spectrum texture.
    double in[targetSamples] = {0.0};

    const unsigned char *currentSample = reinterpret_cast<const unsigned char*>(data);

    for (int i = 0; i < numSamples; i++) {
        for (int j = 0; j < channelCount; j++) {
            // Assume that we only need to sample one channel
            // to get a good fft representation
            if (j > 0) {
                currentSample += channelBytes;
                continue;
            }

            if (i > targetSamples) {
                break;
            }

            // For simplicity we assume that the sample is 16 bit signed pcm input
            const qint16 pcmSample = *reinterpret_cast<const qint16*>(currentSample);

            // Normalize the sample with the max negative value for signed 16 bit: -32768
            in[i] = double(pcmSample) / 32768.0;

            currentSample += channelBytes;
        }
    }

    fftw_complex out[fftwSamples];
    fftw_plan plan = fftw_plan_dft_r2c_1d(targetSamples, in, out, FFTW_ESTIMATE);
    fftw_execute(plan);

    float spectrum[fftwSamples];

    for (int i = 0; i < fftwSamples; i++) {
        double val = qSqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / fftwScale;
        val = val > 1.0 ? 1.0 : val;

        spectrum[i] = val;
    }

    texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, (const void*) &spectrum);

    return len;
}

QOpenGLTexture* AudioDevice::spectrumTexture() {
    return texture;
}
