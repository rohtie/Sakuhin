#include "backend.h"

#include "sessionmanager.h"
#include "audiomanager.h"
#include "shadermanager.h"
#include "windowmanager.h"
#include "scenemanager.h"

Backend::Backend(QObject* parent) : QObject(parent) {
    easingNames << "Linear" << "QuadraticIn" << "QuadraticOut" << "QuadraticInOut" << "CubicIn" << "CubicOut" << "CubicInOut" << "QuarticIn" << "QuarticOut" << "QuarticInOut" << "QuinticIn" << "QuinticOut" << "QuinticInOut" << "SineIn" << "SineOut" << "SineInOut" << "CircularIn" << "CircularOut" << "CircularInOut" << "ExponentialIn" << "ExponentialOut" << "ExponentialInOut" << "ElasticIn" << "ElasticOut" << "ElasticInOut" << "BackIn" << "BackOut" << "BackInOut" << "BounceOut" << "BounceIn" << "BounceInOut";
}

void Backend::setPerformanceInformation(const QString &performanceInformation) {
    this->performanceInformation = performanceInformation;

    emit performanceInformationChanged();
}

void Backend::initialize() {
}
