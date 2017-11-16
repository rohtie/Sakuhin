#include "audiodevice.h"

AudioDevice::AudioDevice(QAudioDeviceInfo* deviceInfo) {
    name = deviceInfo->deviceName();
    this->deviceInfo = deviceInfo;

    emit nameChanged();
}
