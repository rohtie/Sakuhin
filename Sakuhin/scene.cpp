#include "scene.h"

Scene::Scene(QObject *parent) : QObject(parent) {

}

Scene::Scene(const QString name, int shaderIndex) {
    this->name = name;
    this->shaderIndex = shaderIndex;
}
