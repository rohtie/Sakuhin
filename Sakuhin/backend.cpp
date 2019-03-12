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

void Backend::initialize(const QSurfaceFormat &format, QObject* qmlRoot) {
    SessionManager* sessionmanager = qmlRoot->findChild<SessionManager*>();
    AudioManager* audiomanager = qmlRoot->findChild<AudioManager*>();
    ShaderManager* shadermanager = qmlRoot->findChild<ShaderManager*>();
    WindowManager* windowmanager = qmlRoot->findChild<WindowManager*>();
    SceneManager* scenemanager = qmlRoot->findChild<SceneManager*>();

    sessionmanager->initialize(shadermanager, scenemanager, windowmanager);
    sessionmanager->createSession();

    audiomanager->initialize();
    shadermanager->initialize(format);
    windowmanager->initialize(format, this, shadermanager, scenemanager);
    scenemanager->initialize(shadermanager);
}
