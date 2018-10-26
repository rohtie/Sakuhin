#include "liveqmlapplicationengine.h"

LiveQmlApplicationEngine::LiveQmlApplicationEngine(QObject *parent) : QQmlApplicationEngine(parent) {

}

void LiveQmlApplicationEngine::clearCache() {
    this->clearComponentCache();
}
