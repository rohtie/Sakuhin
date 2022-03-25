#include "queuemanager.h"

#include <QJsonObject>
#include <QJsonArray>

#include "shadermanager.h"
#include "scene.h"

QueueManager::QueueManager(QObject *parent) : QObject(parent) {

}

void QueueManager::initialize(ShaderManager* shadermanager, const QJsonArray &jsonQueue) {
    this->shadermanager = shadermanager;

    QObject::connect(shadermanager, &ShaderManager::mainIndexChanged,
                     this, &QueueManager::onShaderIndexChange);

    // Load queue scenes from json
    for (int i=0; i<jsonQueue.count(); i++) {
        QJsonObject jsonScene = jsonQueue.at(i).toObject();

        Scene* newScene = new Scene(jsonScene["name"].toString(), jsonScene["shader"].toInt());
        queue.append(newScene);
    }

    emit queueChanged();
}

void QueueManager::onShaderIndexChange() {
    if (queue.count() > 0) {
        Scene* currentScene = (Scene*) queue.at(sceneIndex);

        if (currentScene->shaderIndex != shadermanager->mainIndex) {
            currentScene->shaderIndex = shadermanager->mainIndex;
        }
    }
}

void QueueManager::swap(int indexA, int indexB) {
    queue.swap(indexA, indexB);

    emit queueChanged();
}

void QueueManager::newScene() {
    queue.append(new Scene(QString::number(QDateTime::currentMSecsSinceEpoch()), 0));

    emit queueChanged();
}

void QueueManager::selectScene(int index) {
    sceneIndex = index;

    Scene* currentScene = (Scene*) queue.at(sceneIndex);
    int shaderIndex = currentScene->shaderIndex;

    shadermanager->selectShader(shaderIndex);
    shadermanager->makeCurrent(shaderIndex);

    emit sceneIndexChanged();
}

void QueueManager::previousScene() {
    int a = sceneIndex - 1;
    int b = queue.count();

    int negativeModulus = (b + (a%b)) % b;

    selectScene(negativeModulus);
}

void QueueManager::nextScene() {
    selectScene((sceneIndex + 1) % queue.count());
}
