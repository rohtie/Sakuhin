#include "scene.h"

Scene::Scene(QObject *parent) : QObject(parent) {

}

Scene::Scene(const QString name) {
    this->name = name;
}