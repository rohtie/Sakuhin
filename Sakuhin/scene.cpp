#include "scene.h"

#include <QJsonObject>

Scene::Scene(QObject *parent) : QObject(parent) {

}

Scene::Scene(const QString name, int shaderIndex) {
    this->name = name;
    this->shaderIndex = shaderIndex;
}

QJsonObject* Scene::toJson() {
    QJsonObject* jsonScene = new QJsonObject();
    (*jsonScene)["name"] = name;
    (*jsonScene)["length"] = length;
    (*jsonScene)["shader"] = shaderIndex;

    return jsonScene;
}
