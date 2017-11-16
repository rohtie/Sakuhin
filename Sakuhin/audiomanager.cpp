#include "audiomanager.h"
#include "audiodevice.h"

#include <QDebug>

AudioManager::AudioManager(QObject *parent) : QObject(parent) {
}

void AudioManager::initialize() {
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    for (int i = 0; i < devices.size(); i++) {
        audioDevices.append(new AudioDevice(devices[i]));
    }

    emit audioDevicesChanged();
}
