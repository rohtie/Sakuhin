#include "scenemanager.h"

#include "scene.h"

SceneManager::SceneManager(QObject *parent) : QObject(parent) {

}

void SceneManager::initialize() {
    scenes.append(new Scene("Intro"));
    scenes.append(new Scene("Landscape"));
    scenes.append(new Scene("Rolling trees"));
    scenes.append(new Scene("Geometric balls"));
    scenes.append(new Scene("Outro"));

    scenesChanged();
}

void SceneManager::swap(int indexA, int indexB) {
    scenes.swap(indexA, indexB);

    scenesChanged();
}
